#include "zombiewindow.h"
#include "zombiegame.h"
#include "configuration.h"

namespace zombie {

	namespace {

		void PushButtonStyle() {
			ImGui::PushStyleColor(ImGuiCol_Text, sdl::color::html::Black);
			ImGui::PushStyleColor(ImGuiCol_Border, sdl::color::Transparent);
			ImGui::PushStyleColor(ImGuiCol_Button, sdl::color::html::White);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, sdl::color::html::Red);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, sdl::color::html::RosyBrown);
		}

		void PopButtonStyle() {
			ImGui::PopStyleColor(5);
			//ImGui::PopFont();
		}

	}

	ZombieWindow::ZombieWindow(bool skipMenu)
		: skipMenu_{skipMenu} {

		sdl::Window::setPosition(Configuration::getInstance().getWindowPositionX(), Configuration::getInstance().getWindowPositionY());
		sdl::Window::setSize(Configuration::getInstance().getWindowWidth(), Configuration::getInstance().getWindowHeight());
		sdl::Window::setIcon(Configuration::getInstance().getWindowIcon());
		//sdl::Window::setResizeable(Configuration::getInstance().isWindowResizable());
		//sdl::Window::setFullScreen(Configuration::getInstance().isWindowMaximized());
		//sdl::Window::setMinSize(400, 400);

		// sdl::ImGuiWindow::setShowDemoWindow(true);
	}

	ZombieWindow::~ZombieWindow() {
		Configuration::getInstance().close();
	}

	void ZombieWindow::processEvent(const SDL_Event& windowEvent) {
		if (zombieGame_) {
			zombieGame_->eventUpdate(windowEvent);
		}

		switch (windowEvent.type) {
			case SDL_EVENT_WINDOW_RESIZED:
				if (!(SDL_GetWindowFlags(sdl::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
					// The Window's is not maximized. Save size!
					//GameData::getInstance().setWindowWidth(windowEvent.window.data1);
					//GameData::getInstance().setWindowHeight(windowEvent.window.data2);
				}
				break;
			case SDL_EVENT_WINDOW_MOVED:
				if (!(SDL_GetWindowFlags(sdl::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
					// The Window's is not maximized. Save position!
					//const auto [x, y] = sdl::Window::getSize();
					//GameData::getInstance().setWindowPositionX(x);
					//GameData::getInstance().setWindowPositionY(y);
				}
				break;
			case SDL_EVENT_WINDOW_MAXIMIZED:
				//GameData::getInstance().setWindowMaximized(true);
				break;
			case SDL_EVENT_WINDOW_RESTORED:
				//GameData::getInstance().setWindowMaximized(false);
				break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				//GameData::getInstance().setWindowMaximized(false);
				quit();
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				break;
			case SDL_EVENT_KEY_UP:
				break;
				if (zombieGame_) {
			case SDL_EVENT_KEY_DOWN:
					switch (windowEvent.key.key) {
						case SDLK_ESCAPE:
							sdl::Window::quit();
							break;
						case SDLK_PAGEUP:
							zombieGame_->zoom(1.1f);
							break;
						case SDLK_PAGEDOWN:
							zombieGame_->zoom(1 / 1.1f);
							break;
						case SDLK_RETURN:
							break;
						case SDLK_P: [[fallthrough]];
						case SDLK_PAUSE:
							break;
					}
				}
				break;
		}

	}

	void ZombieWindow::renderFrame(const sdl::DeltaTime& deltaTime, SDL_GPUTexture* swapchainTexture, SDL_GPUCommandBuffer* commandBuffer) {
		if (zombieGame_) {
			int w, h;
			SDL_GetWindowSize(window_, &w, &h);
			zombieGame_->setSize(w, h);
		}

		if (zombieGame_) {
			graphic_.clear();

			//shader_.pushProjectionMatrix(commandBuffer, modelMatrix);
			auto delta = std::chrono::duration<double>(deltaTime).count();
			zombieGame_->draw(graphic_, delta);

		} else {
			graphic_.clear();
			graphic_.addRectangleImage({-1, -1}, {2, 2}, background_);
		}
		graphic_.uploadToGpu(gpuDevice_, commandBuffer);

		SDL_GPUColorTargetInfo targetInfo{
			.texture = swapchainTexture,
			.clear_color = clearColor_,
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE,
		};
		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
		graphic_.bindBuffers(renderPass);
		if (zombieGame_) {
			shader_.uploadProjectionMatrix(commandBuffer, graphic_.getMatrix());
			SDL_SetGPUViewport(renderPass, zombieGame_->getViewport());
		} else {
			int w, h;
			SDL_GetWindowSize(window_, &w, &h);
			SDL_GPUViewport viewPort{
				.x = 0,
				.y = 0,
				.w = static_cast<float>(w),
				.h = static_cast<float>(h),
				.min_depth = 0.0f,
				.max_depth = 1.0f
			};
			shader_.uploadProjectionMatrix(commandBuffer, glm::mat4{1.f});
			SDL_SetGPUViewport(renderPass, &viewPort);
		}
		SDL_BindGPUGraphicsPipeline(renderPass, graphicsPipeline_.get());
		SDL_BindGPUFragmentSamplers(
			renderPass,
			0,
			Configuration::getInstance().getTextureSamplerBinding(),
			1
		);
		graphic_.draw(renderPass);
		SDL_EndGPURenderPass(renderPass);
	}

	void ZombieWindow::renderImGui(const sdl::DeltaTime& deltaTime) {
		if (zombieGame_) {
			ImGui::PushFont(debugFont_, 12.f);
			zombieGame_->imGuiUpdate(deltaTime);
			ImGui::PopFont();
			return;
		}

		ImGui::MainWindow("Main", [&]() {
			ImGui::Indent(10.f);
			ImGui::PushFont(titleFont_, 45.0f);
			ImGui::TextColored(sdl::color::html::White, "Zombie");
			ImGui::PopFont();

			PushButtonStyle();
			if (ImGui::Button("Play")) {
				zombieGame_ = std::make_unique<ZombieGame>();
			}

			if (ImGui::Button("Custom game")) {

			}

			if (ImGui::Button("Highscore")) {

			}

			if (ImGui::Button("Quit")) {
				quit();
			}
			PopButtonStyle();
		});
	}

	void ZombieWindow::preLoop() {
		Configuration::getInstance().init(gpuDevice_);
		shader_.load(gpuDevice_);

		// describe the vertex buffers
		SDL_GPUVertexBufferDescription vertexBufferDescriptions{
			.slot = 0,
			.pitch = sizeof(sdl::Vertex),
			.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX
		};

		SDL_GPUColorTargetDescription colorTargetDescription{
			.format = SDL_GetGPUSwapchainTextureFormat(gpuDevice_, getSdlWindow()),
			.blend_state = SDL_GPUColorTargetBlendState{
				.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.color_blend_op = SDL_GPU_BLENDOP_ADD,
				.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
				.enable_blend = true
			}
		};

		SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{
			.vertex_shader = shader_.vertexShader.get(),
			.fragment_shader = shader_.fragmentShader.get(),
			.vertex_input_state = SDL_GPUVertexInputState{
				.vertex_buffer_descriptions = &vertexBufferDescriptions,
				.num_vertex_buffers = 1,
				.vertex_attributes = shader_.attributes.data(),
				.num_vertex_attributes = (Uint32) shader_.attributes.size()
			},
			.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
			.target_info = SDL_GPUGraphicsPipelineTargetInfo{
				.color_target_descriptions = &colorTargetDescription,
				.num_color_targets = 1,
			}
		};
		graphicsPipeline_ = sdl::createGpuGraphicsPipeline(gpuDevice_, pipelineInfo);
		
		background_ = Configuration::getInstance().getMenuBackgroundImage();

		buttonFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 30);
		titleFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 45);
		debugFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 12);
	}

}
