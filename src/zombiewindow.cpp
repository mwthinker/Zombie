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
		sdl::Window::setResizeable(Configuration::getInstance().isWindowResizable());
		sdl::Window::setFullScreen(Configuration::getInstance().isWindowMaximized());
		sdl::Window::setMinSize(400, 400);

        // sdl::ImGuiWindow::setShowDemoWindow(true);
	}

	ZombieWindow::~ZombieWindow() {
	}

	void ZombieWindow::imGuiEventUpdate(const SDL_Event& windowEvent) {
		if (zombieGame_) {
			zombieGame_->eventUpdate(windowEvent);
		}

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						if (!(SDL_GetWindowFlags(sdl::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
							// The Window's is not maximized. Save size!
							//GameData::getInstance().setWindowWidth(windowEvent.window.data1);
							//GameData::getInstance().setWindowHeight(windowEvent.window.data2);
						}
						break;
					case SDL_WINDOWEVENT_MOVED:
						if (!(SDL_GetWindowFlags(sdl::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
							// The Window's is not maximized. Save position!
							//const auto [x, y] = sdl::Window::getSize();
							//GameData::getInstance().setWindowPositionX(x);
							//GameData::getInstance().setWindowPositionY(y);
						}
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						//GameData::getInstance().setWindowMaximized(true);
						break;
					case SDL_WINDOWEVENT_RESTORED:
						//GameData::getInstance().setWindowMaximized(false);
						break;
					case SDL_WINDOWEVENT_CLOSE:
						//GameData::getInstance().setWindowMaximized(false);
						quit();
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_KEYUP:
				break;
			case SDL_KEYDOWN:
				if (zombieGame_) {
					switch (windowEvent.key.keysym.sym) {
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
						case SDLK_p: [[fallthrough]];
						case SDLK_PAUSE:
							break;
					}
				}
				break;
		}

	}

	void ZombieWindow::imGuiPreUpdate(const sdl::DeltaTime& deltaTime) {
		if (zombieGame_) {
			Viewport viewport;
			viewport.x = 0;
			viewport.y = 0;
			viewport.w = getWidth();
			viewport.h = getHeight();
			
			zombieGame_->setSize(getWidth(), getHeight(), viewport);
		}

		shader_.useProgram();
		if (zombieGame_) {
			auto delta = std::chrono::duration<double>(deltaTime).count();
			zombieGame_->draw(graphic_, delta);
		} else {
			shader_.useProgram();
			graphic_.clear();
			graphic_.addRectangleImage({-1, -1}, {2, 2}, background_);
		}
		graphic_.upload(shader_);
	}

	void ZombieWindow::imGuiUpdate(const sdl::DeltaTime& deltaTime) {
		if (zombieGame_) {
			ImGui::PushFont(debugFont_);
			zombieGame_->imGuiUpdate(deltaTime);
			ImGui::PopFont();
			return;
		}

		ImGui::MainWindow("Main", [&]() {
			ImGui::Indent(10.f);
			ImGui::PushFont(titleFont_);
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

	void ZombieWindow::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void ZombieWindow::initPreLoop() {
		sdl::ImGuiWindow::initPreLoop();

		shader_ = sdl::Shader::CreateShaderGlsl_330();
		Configuration::getInstance().bindTextureFromAtlas();
		background_ = Configuration::getInstance().getMenuBackgroundImage();

		buttonFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 30);
		titleFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 45);
		debugFont_ = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 12);
	}

}
