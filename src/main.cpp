#include "logger.h"
#include "zombiewindow.h"

int main (int argc, char** argv) {
	spdlog::info("[main] Zombie Version:  {}", PROJECT_VERSION);
	spdlog::info("[main] Zombie git hash:  {}", GIT_VERSION);

	bool skipMenu = false;
	if (argc > 1) {
		skipMenu = true;
	}

	zombie::ZombieWindow zombieWindow{skipMenu};
	zombieWindow.startLoop();

	return 0;
}
