#include "logger.h"
#include "zombiewindow.h"
#include "sdl/initsdl.h"

int main (int argc, char** argv) {
	zombie::logger::init();
	spdlog::info("[main] Zombie Version:  {}", PROJECT_VERSION);
	spdlog::info("[main] Zombie git hash:  {}", GIT_VERSION);

	bool skipMenu = false;
	if (argc > 1) {
		skipMenu = true;
	}

	const sdl::InitSdl SDL;
	zombie::ZombieWindow zombieWindow{skipMenu};
	zombieWindow.startLoop();

    return 0;
}
