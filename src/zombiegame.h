#ifndef ZOMBIE_ZOMBIEGAME_H
#define ZOMBIE_ZOMBIEGAME_H

#include "physicengine.h"
#include "gameinterface.h"
#include "device.h"

#include "Physic/Moving/unit.h"
#include "Physic/Moving/unitproperties.h"

#include "player.h"
#include "gameobject.h"

#include <sdl/graphic.h>
#include <sdl/sprite.h>
#include <sdl/sound.h>
#include <sdl/music.h>

#include <memory>
#include <vector>

namespace zombie {

	// Responsible of loading map, units and initiate all
	// game related things and to start the game engine.
	// It also handle all game events triggered by the game engine,
	// e.g. what happens when a unit dies. All graphics and sound is
	// started here.
	class ZombieGame {
	public:
		ZombieGame();
		~ZombieGame();
		
		void draw(sdl::Graphic& graphic, double deltaTime);

		void zoom(float scale);

		void eventUpdate(const SDL_Event& windowEvent);

	private:
		class Game : public GameInterface {
		public:
			Game(ZombieGame& zombieGame)
				: zombieGame_{zombieGame} {
			}

			void unitDied(Unit& unit) override;
			void collision(float impulse, Unit& unit) override;
			void collision(float impulse, Car& car) override;
			void collision(float impulse, Building& building) override;
			void shotMissed(Position startPosition, Position hitPosition) override;
			void shotHit(Position startPosition, Position hitPosition, Unit& unit) override;
			void explosion(Position position, float explosionRadius) override;
			void removedFromWorld(Unit& unit) override;

		private:
			ZombieGame& zombieGame_;
		};

		void updateGame(double deltaTime);

		void makeGameStep();

		void zombieGameInit();
		
		void unitDied(Unit& unit);

		void collision(float impulse, Unit& unit);

		void collision(float impulse, Car& car);

		void collision(float impulse, Building& building);

		void shotMissed(Position startPosition, Position hitPosition);

		void shotHit(Position startPosition, Position hitPosition, Unit& unit);

		void explosion(Position position, float explosionRadius);

		void removedFromWorld(Unit& unit);
		
		glm::mat4 worldToCamera_;
		glm::mat4 cameraToScreen_;

		Game game_;
		sdl::Music music_;

		Position viewPosition_;
		Position refViewPosition_;

		DevicePtr keyboard_;

		std::vector<std::unique_ptr<Player>> players_;
		PhysicEngine engine_;

		// Fix timestep.
		double timeStep_{1.0/60.0};
		double accumulator_{};
	};

}

#endif
