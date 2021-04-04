#include "zombiewindow.h"
#include "zombiegame.h"
#include "configuration.h"

namespace zombie {

	ZombieWindow::ZombieWindow(bool skipMenu)
		: skipMenu_{skipMenu} {

		sdl::Window::setPosition(Configuration::getInstance().getWindowPositionX(), Configuration::getInstance().getWindowPositionY());
		sdl::Window::setSize(Configuration::getInstance().getWindowWidth(), Configuration::getInstance().getWindowHeight());
		sdl::Window::setIcon(Configuration::getInstance().getWindowIcon());
		sdl::Window::setResizeable(Configuration::getInstance().isWindowResizable());
		sdl::Window::setFullScreen(Configuration::getInstance().isWindowMaximized());
		sdl::Window::setMinSize(400, 400);
	}

	ZombieWindow::~ZombieWindow() {
		//zombieEntry_.save();
	}

	void ZombieWindow::imGuiEventUpdate(const SDL_Event& windowEvent) {
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
							const auto [x, y] = sdl::Window::getSize();
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
			case SDL_KEYUP:
				zombieGame_->eventUpdate(windowEvent);
				break;
			case SDL_KEYDOWN:
				zombieGame_->eventUpdate(windowEvent);
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
				break;
		}

	}

	void ZombieWindow::imGuiPreUpdate(const sdl::DeltaTime& deltaTime) {
		auto deltaTimeSeconds = std::chrono::duration<double>(deltaTime).count();
		//zombieGame_->draw(getShader(), graphic_, deltaTimeSeconds);
	}

	void ZombieWindow::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void ZombieWindow::initPreLoop() {
		sdl::ImGuiWindow::initPreLoop();

		zombieGame_ = std::make_unique<ZombieGame>();

		zombieGame_->startGame();
	}

}
