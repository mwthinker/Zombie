#ifndef ZOMBIE_ZOMBIEWINDOW_H
#define ZOMBIE_ZOMBIEWINDOW_H

#include <sdl/imguiwindow.h>
#include <sdl/graphic.h>
#include <sdl/shader.h>

#include <memory>

namespace zombie {

	class GameData;
	class ZombieGame;

	class ZombieWindow : public sdl::ImGuiWindow {
	public:
		explicit ZombieWindow(bool skipMenu = false);

		~ZombieWindow();

	private:
		void imGuiPreUpdate(const sdl::DeltaTime& deltaTime) override;
		void imGuiUpdate(const sdl::DeltaTime& deltaTime) override;

		void imGuiEventUpdate(const SDL_Event& windowEvent) override;

		void initOpenGl() override;
		void initPreLoop() override;

		std::unique_ptr<ZombieGame> zombieGame_;
		bool skipMenu_{};
		sdl::Graphic graphic_;
		sdl::Shader shader_;
		sdl::TextureView background_;
		ImFont* titleFont_{};
		ImFont* buttonFont_{};
		ImFont* debugFont_{};
	};

}

#endif
