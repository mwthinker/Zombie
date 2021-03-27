#ifndef ZOMBIE_ZOMBIEWINDOW_H
#define ZOMBIE_ZOMBIEWINDOW_H

#include <sdl/imguiwindow.h>
#include <Graphic/graphic.h>

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

		void imGuiEventUpdate(const SDL_Event& windowEvent) override;

		void initOpenGl() override;
		void initPreLoop() override;

		std::unique_ptr<ZombieGame> zombieGame_;
		bool skipMenu_{};
		graphic::Graphic graphic_;
	};

}

#endif
