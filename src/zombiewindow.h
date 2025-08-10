#ifndef ZOMBIE_ZOMBIEWINDOW_H
#define ZOMBIE_ZOMBIEWINDOW_H

#include "graphic.h"

#include <sdl/window.h>
#include <sdl/shader.h>
#include <sdl/gpu/sdlgpu.h>

#include <memory>

namespace zombie {

	class GameData;
	class ZombieGame;

	class ZombieWindow : public sdl::Window {
	public:
		explicit ZombieWindow(bool skipMenu = false);

		~ZombieWindow() override;

	private:
		void renderFrame(const sdl::DeltaTime& deltaTime, SDL_GPUTexture* swapchainTexture, SDL_GPUCommandBuffer* commandBuffer) override;
		void renderImGui(const sdl::DeltaTime& deltaTime) override;

		void processEvent(const SDL_Event& windowEvent) override;

		void preLoop() override;

		std::unique_ptr<ZombieGame> zombieGame_;
		bool skipMenu_ = false;
		Graphic graphic_;
		sdl::Shader shader_;
		TextureView background_;
		ImFont* titleFont_ = nullptr;
		ImFont* buttonFont_ = nullptr;
		ImFont* debugFont_ = nullptr;

		sdl::gpu::GpuGraphicsPipeline graphicsPipeline_;
	};

}

#endif
