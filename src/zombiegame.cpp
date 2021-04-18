#include "zombiegame.h"
#include "auxiliary.h"
#include "inputkeyboard.h"

#include "physics/weapons/shot.h"
#include "physics/weapons/gun.h"
#include "physics/weapons/weapon.h"

#include "humanplayer.h"
#include "zombieplayer.h"
#include "configuration.h"

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

	}

	namespace factory {

		HumanPlayerPtr createHumanPlayer(PhysicEngine& physicEngine, const UnitProperties& properties, DevicePtr device, Position pos = Zero) {
			WeaponPtr weapon;
			auto unit = std::make_unique<Unit>(properties, weapon);
			physicEngine.add(unit.get());
			unit->setState(State{pos, Zero, 0.f, 0.f});
			unit->setEnabled(true);
			unit->setAwake(true);
			return std::make_unique<HumanPlayer>(device, std::move(unit));
		}

		ZombiePlayerPtr createZombiePlayer(PhysicEngine& physicEngine, const UnitProperties& properties, Position pos = Zero) {
			WeaponPtr weapon;
			auto unit = std::make_unique<Unit>(properties, weapon);
			physicEngine.add(unit.get());
			unit->setState(State{pos, Zero, 0.f, 0.f});
			unit->setEnabled(true);
			unit->setAwake(true);
			return std::make_unique<ZombiePlayer>(std::move(unit));
		}

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

	ZombieGame::ZombieGame()
		: game_{*this}
		, engine_{game_, Configuration::getInstance().getSettingsImpulseThreshold()}
		, timeStep_{Configuration::getInstance().getSettingsTimeStep()}
		, accumulator_{0} {

		zombieGameInit();
	}

	ZombieGame::~ZombieGame() {
	}

	void ZombieGame::eventUpdate(const SDL_Event& windowEvent) {
		keyboard_->eventUpdate(windowEvent);

		switch (windowEvent.type) {
			case SDL_MOUSEBUTTONDOWN:
				switch (windowEvent.button.button) {
					case SDL_BUTTON_LEFT: {
						auto pos = glm::inverse(worldToCamera_) * glm::inverse(cameraToClip_) * pixelToClip_ * glm::vec4{windowEvent.button.x, windowEvent.button.y, 0.f, 1.f};
						drawDebugCircle_.position = {pos.x, pos.y};
						break;
					}
				}
				break;
			case SDL_MOUSEWHEEL:
				if (windowEvent.wheel.y > 0) {
					zoom(1.1f);
				}
				if (windowEvent.wheel.y < 0) {
					zoom(0.9f);
				}
				break;
			case SDL_MOUSEMOTION:
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
					auto delta = glm::inverse(cameraToClip_) * pixelToClip_ * glm::vec4{windowEvent.motion.xrel, windowEvent.motion.yrel, 0.f, 0.f};
					cameraToClip_ = glm::translate(cameraToClip_, {delta.x, delta.y, 0.f});
				}
				break;
		}
	}

	void ZombieGame::zombieGameInit() {
		keyboard_ = std::make_shared<InputKeyboard>(createDefaultKeyboardKeys());

		if (Configuration::getInstance().isMusicOn()) {
			music_ = Configuration::getInstance().getMusicTrack();
			music_.setVolume(Configuration::getInstance().getMusicVolume());
			music_.play(-1);
		}

		worldToCamera_ = glm::mat4{1};
		cameraToClip_ = glm::ortho(-10.f, 10.f, -10.f, 10.f);
		keyboard_ = std::make_shared<InputKeyboard>(createDefaultKeyboardKeys());

		auto humanProperties = Configuration::getInstance().getHumanProperties();
		players_.push_back(factory::createHumanPlayer(engine_, humanProperties, keyboard_));
	}

	void ZombieGame::setPixelToClipMatrix(const glm::mat4& pixelToClip) {
		pixelToClip_ = pixelToClip;
	}

	void ZombieGame::draw(sdl::Graphic& graphic, double deltaTime) {
		updateGame(deltaTime);

		graphic.clear();
		graphic.addRectangle({-1, -1}, {2, 2}, sdl::color::html::OliveDrab);
		graphic.setMatrix(cameraToClip_ * worldToCamera_);
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				float x = i - 50.f;
				float y = j - 50.f;
				const float w = 0.05f;
				graphic.addRectangle({x + w, y + w}, {1.f - w * 2.f, 1.f - w * 2.f}, sdl::color::html::ForestGreen);
			}
		}
		for (auto& player : players_) {
			player->draw(graphic);
		}

		drawDebugCircle_.draw(graphic);
	}

	void ZombieGame::imGuiUpdate(const sdl::DeltaTime& deltaTime) {
		ImGui::Window("Factory", [&]() {
			if (ImGui::Button("Create Zombie")) {
				players_.push_back(factory::createZombiePlayer(engine_, UnitProperties{}, drawDebugCircle_.position));
			}
		});
	}

	void ZombieGame::updateGame(double deltaTime) {
		if (deltaTime > 0.25) {
			// To avoid spiral of death.
			deltaTime = 0.25;
		}

		accumulator_ += deltaTime;
		while (accumulator_ >= timeStep_) {
			accumulator_ -= timeStep_;
			makeGameStep();
		}
	}

	void ZombieGame::makeGameStep() {
		engine_.update(timeStep_);

		auto time = engine_.getTime();
		for (auto& player : players_) {
			player->updateInput(time, timeStep_);
		}
	}

	void ZombieGame::zoom(float scale) {
		cameraToClip_ = glm::scale(cameraToClip_, {scale, scale, 1});
	}

	void ZombieGame::unitDied(Unit& unit) {
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

	void ZombieGame::removedFromWorld(Unit& unit) {
	}

}
