#include "zombiegame.h"
#include "auxiliary.h"
#include "inputkeyboard.h"

#include "Weapons/shot.h"
#include "Weapons/gun.h"

#include "humanplayer.h"
#include "zombiebehavior.h"
#include "gamedata.h"

#include <sdl/opengl.h>
#include <sdl/exception.h>

#include <cmath>
#include <sstream>

namespace zombie {

	namespace {

		// Return a random postion between the defined outer and inner circle centered in position.
		Position generatePosition(Position position, float innerRadius, float outerRadius) {
			float angle = random(0, 2.f * Pi);
			return position + (innerRadius + (outerRadius - innerRadius) * random()) * Position(std::cos(angle), std::sin(angle));
		}
		
		Position generatePosition(const std::vector<Position>& positions) {
			if (positions.size() > 0) {
				return positions[randomInt(0, static_cast<int>(positions.size() - 1))];
			}
			return Zero;
		}
		
		/*
		template <class Vector>
		void activateFirstFreeSlot(Vector& v, Position p, float angle, const Animation& dieAnimation) {
			// Activate the first free animation slot.
			for (GraphicAnimation& animation : v) {
				if (animation.toBeRemoved()) {
					animation.restart(p, angle, dieAnimation);
					break;
				}
			}
		}
		*/

		constexpr KeyboardKeys createDefaultKeyboardKeys() {
			KeyboardKeys keyboardKeyCodes{};
			keyboardKeyCodes.up = SDLK_UP;
			keyboardKeyCodes.down = SDLK_DOWN;
			keyboardKeyCodes.left = SDLK_LEFT;
			keyboardKeyCodes.right = SDLK_RIGHT;
			keyboardKeyCodes.shoot = SDLK_SPACE;
			keyboardKeyCodes.reload = SDLK_r;
			keyboardKeyCodes.run = SDLK_LSHIFT;
			keyboardKeyCodes.action = SDLK_e;
			return keyboardKeyCodes;
		}

		class SdlEvent : public Event {
		public:
			SdlEvent(const SDL_Event& windowEvent)
				: windowEvent_{windowEvent_} {

			}

		private:
			SDL_Event windowEvent_;
		};

	}

	void ZombieGame::Game::unitDied(Unit& unit) {
		zombieGame_.unitDied(unit);
	}

	void ZombieGame::Game::collision(float impulse, Unit& unit) {
		zombieGame_.collision(impulse, unit);
	}

	void ZombieGame::Game::collision(float impulse, Car& car) {
		zombieGame_.collision(impulse, car);
	}

	void ZombieGame::Game::collision(float impulse, Building& building) {
		zombieGame_.collision(impulse, building);
	}

	void ZombieGame::Game::shotMissed(Position startPosition, Position hitPosition) {
		zombieGame_.shotMissed(startPosition, hitPosition);
	}

	void ZombieGame::Game::shotHit(Position startPosition, Position hitPosition, Unit& unit) {
		zombieGame_.shotHit(startPosition, hitPosition, unit);
	}

	void ZombieGame::Game::explosion(Position position, float explosionRadius) {
		zombieGame_.explosion(position, explosionRadius);
	}

	void ZombieGame::Game::removedFromWorld(Unit& unit) {
		zombieGame_.removedFromWorld(unit);
	}

	void ZombieGame::Game::shot(Unit& shooter, float speed, float explodeTime, float damage, float explosionRadius, float force) {
		zombieGame_.shot(shooter, speed, explodeTime, damage, explosionRadius, force);
	}

	ZombieGame::ZombieGame()
		: game_{*this}
		, engine_{game_, GameData::getInstance().getSettingsImpulseThreshold()}
		, timeStep_{GameData::getInstance().getSettingsTimeStep()}
		, accumulator_{0}
		, started_{false} {

		zombieGameInit();
	}

	ZombieGame::~ZombieGame() {
		//zombieEntry_.save();
	}

	void ZombieGame::eventUpdate(const SDL_Event& windowEvent) {
		keyboard_->eventUpdate(windowEvent);
	}

	void ZombieGame::zombieGameInit() {
		keyboard_ = std::make_shared<InputKeyboard>(createDefaultKeyboardKeys());
		clipsize_ = 0;
		bulletsInWeapon_ = 0;
		health_ = 0;
		scale_ = 1.f;
		lastSpawnTime_ = (float) engine_.getTime();
		spawnPeriod_ = 0.5f;

		/*
		addKeyListener([&](gui::Component& component, const SDL_Event& keyEvent) {
			keyboard_->eventUpdate(keyEvent);
		});
		*/

		/*
		if (GameData::getInstance().isMusicOn()) {
			music_ = GameData::getInstance().getMusicTrack();
			music_.setVolume(GameData::getInstance().getMusicVolume());
			music_.play(-1);
		}
		*/
		
		nbrUnits_ = 0;

		unitMaxLimit_ = GameData::getInstance().getSettingsUnitLimit();

		innerSpawnRadius_ = GameData::getInstance().getSettingsInnerSpawnRadius();
		outerSpawnRadius_ = GameData::getInstance().getSettingsOuterSpawnRadius();
		
		
		//explosionProperties_ = GameData::getInstance().getExplosionProperties();
		
		//humanProperties_ = GameData::getInstance().getHumanProperties();
		//zombieProperties_ = GameData::getInstance().getZombieProperties();

		//Unit2D human(loadUnit(this, humanProperties_, false));
		//Unit2D zombie(loadUnit(this, zombieProperties_, true));

		// Add human to engine.
		{
			State state{Position(85,120), Origo, 0, 0};
			//Position p = generatePosition(spawningPoints_);
			//State state(Position(200,200), Origo, 0);
			/*
			Unit* unit = units_.pushBack(human);
			engine_.add(unit);
			unit->setState(state);
			unit->setActive(true);
			unit->setAwake(true);
			players_.push_back(std::unique_ptr<HumanPlayer>(new HumanPlayer(keyboard_, unit)));
			viewPosition_ = state.position_;
			refViewPosition_ = viewPosition_;
			++nbrUnits_;
			*/
		}

		// Add zombies to engine.
		//calculateValidSpawningPoints(units_[0]);
		unsigned int unitLevel = GameData::getInstance().getSettingsUnitLevel();
		/*
		for (unsigned int i = 1; i <= unitLevel && i < units_.getMaxSize(); ++i) {
			Position p = generatePosition(vaildSpawningPoints_);
			float angle = calculateAnglePointToPoint(p, units_[0].getPosition());
			State state(p, Origo, angle);
			Unit* unit = units_.pushBack(zombie);
			engine_.add(unit);
			unit->setState(state);
			unit->setActive(true);
			unit->setAwake(true);
			players_.push_back(std::unique_ptr<ZombieBehavior>(new ZombieBehavior(unit)));
		}
		*/
	
		// Add cars to engine.
		/*
		Car2D car; // (zombie::loadCar(zombieEntry_.getDeepChildEntry("car")));
		for (unsigned int i = 0; i < 8 && i < units_.getMaxSize(); ++i) {
			State state(Position(85,130), Origo, 0);
			Car* c = cars_.pushBack(car);
			engine_.add(c);
			c->setState(state);
			c->setActive(true);
			c->setAwake(true);
		}
		*/

		// Add missile to engine.
		/*
		Missile2D missile = loadMissile2D(this, GameData::getInstance().getMissileProperties());
		for (unsigned int i = 0; i < 10 && i < units_.getMaxSize(); ++i) {
			engine_.add(missiles_.emplaceBack(missile));
		}
		*/

		//setBackgroundColor(0, 0.1f, 0);
		zombiesKilled_ = 0;

		//drawBuildings_.createVBO(buildings_, wall_.getTexture());
	}

	void ZombieGame::calculateValidSpawningPoints(Unit& human) {
		vaildSpawningPoints_.clear();
		float inner = 10;
		float outer = 200;
		Position humanPos = human.getPosition();
		for (Position p : spawningPoints_) {
			Position diff = p - humanPos;
			if (diff.LengthSquared() > inner*inner && diff.LengthSquared() < outer*outer) {
				// Spawningpoint is valid!
				vaildSpawningPoints_.push_back(p);
			}
		}
	}
	
	void ZombieGame::moveUnits(Unit& unit, Unit& human) {
		Position diff = unit.getPosition() - human.getPosition();
		double inner = 10;
		double outer = 200;
		if (diff.LengthSquared() > outer * outer) {
			// Move unit if possible.
			if (vaildSpawningPoints_.size() > 0) {
				Position p = vaildSpawningPoints_[randomInt(0, static_cast<int>(vaildSpawningPoints_.size() - 1))];
				float angle = calculateAnglePointToPoint(p, human.getPosition());
				State state{p, Origo, angle, 0};
				unit.setState(state);
			} else {
				// Deactivate.
				unit.setEnabled(false);
				unit.setAwake(false);
			}
		}
	}

	// Starts the game.
	void ZombieGame::startGame() {
		started_ = true;
	}

	void ZombieGame::draw(const sdl::Shader& shader, graphic::Graphic& graphic, double deltaTime) {
		sceneManager_.update(deltaTime);
		
		shader.useProgram();
		graphic.clearDraw();
		graphic.addCircle({}, 0.2f, Red);

		sceneManager_.draw(graphic);

		graphic.draw(shader);
	}

	void ZombieGame::updateGame(double deltaTime) {
		if (deltaTime > 0.25) {
			// To avoid spiral of death.
			deltaTime = 0.25;
		}

		// Previous state for the human in the physic loop.
		State previousState;
		bool physicRan = false;

		accumulator_ += deltaTime;
		while (accumulator_ >= timeStep_) {
			accumulator_ -= timeStep_;
			//previousState = units_[0].getState();
			physicRan = true;
			makeGameStep();
		}

		if (physicRan) {
			const float alpha = (float) (accumulator_ / timeStep_);
			//humanState_ = humanState_ = units_[0].getState();
			humanState_.position_ = alpha * humanState_.position_ + (1.f - alpha) * previousState.position_;
			humanState_.velocity_ = alpha * humanState_.velocity_ + (1.f - alpha) * previousState.velocity_;
		}
	}

	void ZombieGame::makeGameStep() {
		float time = (float) engine_.getTime();

		// Update the human and ai input.
		for (auto& player : players_) {
			player->updateInput(time, timeStep_);
		}

		// Update the engine.
		engine_.update(timeStep_);
	}

	void ZombieGame::zoom(float scale) {
		scale_ *= scale;
	}

	void ZombieGame::unitDied(Unit& unit) {
		--nbrUnits_;
		unit.setEnabled(false);
		unit.setAwake(false);
	}

	void ZombieGame::collision(float impulse, Car& car) {
	}

	void ZombieGame::collision(float impulse, Unit& unit) {
		unit.updateHealthPoint(-60 * impulse * static_cast<float>(timeStep_ / 0.016f));
	}

	void ZombieGame::collision(float impulse, Building& building) {
	}

	void ZombieGame::shotMissed(Position startPosition, Position hitPosition) {
		
	}

	void ZombieGame::shotHit(Position startPosition, Position hitPosition, Unit& unit) {

	}

	void ZombieGame::explosion(Position position, float explosionRadius) {

	}

	void ZombieGame::shot(Unit& shooter, float speed, float explodeTime, float damage, float explosionRadius, float force) {
		/*
		for (auto& missile : missiles_) {
			if (!missile.isActive()) {
				float x = shooter.getGrip().x;
				float y = shooter.getGrip().y;
				float angle = shooter.getDirection();
				float s = std::sin(angle);
				float c = std::cos(angle);
				Position release = Position(c * x - s * y, s * x + c * y);  // Rotates the vector.
				missile.create(shooter.getPosition() + release,
					angle, speed, explodeTime, damage, explosionRadius, force);
				// Only send one missile.
				break;
			}
		}
		*/
	}

	void ZombieGame::removedFromWorld(Unit& unit) {
	}

}
