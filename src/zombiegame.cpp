#include "zombiegame.h"
#include "auxiliary.h"
#include "inputkeyboard.h"

#include "physics/weapons/shot.h"
#include "physics/weapons/gun.h"
#include "physics/weapons/weapon.h"

#include "humanplayer.h"
#include "zombieplayer.h"
#include "carplayer.h"

#include "configuration.h"

#include <sdl/sdlexception.h>

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
			return KeyboardKeys{
				.up = SDLK_UP,
				.down = SDLK_DOWN,
				.right = SDLK_RIGHT,
				.left = SDLK_LEFT,
				.shoot = SDLK_SPACE,
				.reload = SDLK_R,
				.run = SDLK_LSHIFT,
				.action = SDLK_E
			};
		}

		void imguiDebugTools(Settings& settings) {
			bool debug = true;

			ImGui::Window("Tools", &debug, ImGuiWindowFlags_NoCollapse, [&]() {
				if (ImGui::BeginTabBar("ControlTabs", ImGuiTabBarFlags_None)) {
					if (ImGui::BeginTabItem("Controls")) {
						ImGui::Separator();
						ImGui::Checkbox("Debug", &settings.debug);
						ImGui::Separator();
						ImGui::SliderInt("Sub step counter", &settings.subStepCount, 1, 20);
						ImGui::SliderFloat("Hertz", &settings.hertz, 5.0f, 120.0f, "%.0f hz");

						ImGui::Separator();
						
                        ImGui::Checkbox("Use Drawing Bounds", &settings.b2DebugDraw.useDrawingBounds);
						ImGui::Checkbox("Draw Shapes", &settings.b2DebugDraw.drawShapes);
						ImGui::Checkbox("Draw Joints", &settings.b2DebugDraw.drawJoints);
						ImGui::Checkbox("Draw Joint Extras", &settings.b2DebugDraw.drawJointExtras);
						ImGui::Checkbox("Draw Bounds", &settings.b2DebugDraw.drawBounds);
						ImGui::Checkbox("Draw Mass", &settings.b2DebugDraw.drawMass);
						ImGui::Checkbox("Draw Body Names", &settings.b2DebugDraw.drawBodyNames);
						ImGui::Checkbox("Draw Contacts", &settings.b2DebugDraw.drawContacts);
						ImGui::Checkbox("Draw Graph Colors", &settings.b2DebugDraw.drawGraphColors);
						ImGui::Checkbox("Draw Contact Normals", &settings.b2DebugDraw.drawContactNormals);
						ImGui::Checkbox("Draw Contact Impulses", &settings.b2DebugDraw.drawContactImpulses);
						ImGui::Checkbox("Draw Contact Features", &settings.b2DebugDraw.drawContactFeatures);
						ImGui::Checkbox("Draw Friction Impulses", &settings.b2DebugDraw.drawFrictionImpulses);
						ImGui::Checkbox("Draw Islands", &settings.b2DebugDraw.drawIslands);
						ImVec2 button_sz{-1, 0};
						if (ImGui::Button("Pause (P)", button_sz)) {
							settings.pause = !settings.pause;
						}

						if (ImGui::Button("Single Step (O)", button_sz)) {
							settings.singleStep = !settings.singleStep;
						}

						if (ImGui::Button("Restart (R)", button_sz)) {
							//RestartTest();
						}

						if (ImGui::Button("Quit", button_sz)) {
							//glfwSetWindowShouldClose(g_mainWindow, GL_TRUE);
						}

						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			});
		}

	}

	namespace factory {

		HumanPlayerPtr createHumanPlayer(PhysicEngine& physicEngine, const UnitProperties& properties, DevicePtr device, Position pos = Zero) {
			WeaponPtr weapon;
			auto unit = std::make_unique<Unit>(properties, weapon);
			physicEngine.add(*unit);
			unit->setState(State{pos, Zero, 0.f, 0.f});
			unit->setEnabled(true);
			unit->setAwake(true);
			return std::make_unique<HumanPlayer>(device, std::move(unit));
		}

		ZombiePlayerPtr createZombiePlayer(PhysicEngine& physicEngine, const UnitProperties& properties, Position pos = Zero) {
			WeaponPtr weapon;
			auto unit = std::make_unique<Unit>(properties, weapon);
			physicEngine.add(*unit);
			unit->setState(State{pos, Zero, random(0.f, 2*Pi), 0.f});
			unit->setEnabled(true);
			unit->setAwake(true);
			return std::make_unique<ZombiePlayer>(std::move(unit));
		}

		CarPlayerPtr createCarPlayer(PhysicEngine& physicEngine, const CarProperties& properties, DevicePtr device, Position pos = Zero) {
			auto car = std::make_unique<Car>(properties);
			physicEngine.add(*car);
			car->setState(State{pos, Zero, random(0.f, 2 * Pi), 0.f});
			car->setEnabled(true);
			car->setAwake(true);
			return std::make_unique<CarPlayer>(device, std::move(car));
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
		, engine_{game_, Configuration::getInstance().getSettingsImpulseThreshold(), settings_.b2DebugDraw}
		, timeStep_{Configuration::getInstance().getSettingsTimeStep()} {

		zombieGameInit();
	}

	ZombieGame::~ZombieGame() {
	}

	void ZombieGame::eventUpdate(const SDL_Event& windowEvent) {
		keyboard_->eventUpdate(windowEvent);

		switch (windowEvent.type) {
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				switch (windowEvent.button.button) {
					case SDL_BUTTON_LEFT: {
						auto pos = getMatrix(Space::Screen, Space::World) * glm::vec4{windowEvent.button.x, windowEvent.button.y, 0.f, 1.f};
						drawDebugArrow_.position = {pos.x, pos.y};
						auto unit = engine_.query<Unit>(drawDebugArrow_.position);
						if (unit != nullptr) {
							spdlog::info("Found unit!");
						} else {
							spdlog::info("Unit not found!");
						}
						break;
					}
				}
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				if (windowEvent.wheel.y > 0) {
					zoom(1.1f);
				}
				if (windowEvent.wheel.y < 0) {
					zoom(0.9f);
				}
				break;
			case SDL_EVENT_MOUSE_MOTION:
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) {
					auto delta = getMatrix(Space::Screen, Space::World) * glm::vec4{windowEvent.motion.xrel, windowEvent.motion.yrel, 0.f, 0.f};
					worldToCamera_ = glm::translate(worldToCamera_, {delta.x, delta.y, 0.f});
				}
				break;
		}
	}

	glm::mat4 ZombieGame::getMatrix(Space from, Space to) const {
		switch (from) {
			case Space::World:
				switch (to) {
					case Space::Camera:
						return worldToCamera_;
					case Space::Clip:
						return cameraToClip_ * worldToCamera_;
					case Space::World:
						return glm::mat4{1};
					case Space::Screen:
						return glm::inverse(screenToClip_) * cameraToClip_ * worldToCamera_;
				}
				break;
			case Space::Camera:
				switch (to) {
					case Space::Camera:
						return glm::mat4{1};
					case Space::Clip:
						return cameraToClip_;
					case Space::World:
						return glm::inverse(worldToCamera_);
					case Space::Screen:
						return glm::inverse(screenToClip_) * cameraToClip_;
				}
				break;
			case Space::Clip:
				switch (to) {
					case Space::Camera:
						return glm::inverse(cameraToClip_);
					case Space::Clip:
						return glm::mat4{1};
					case Space::World:
						return glm::inverse(worldToCamera_) * cameraToClip_;
					case Space::Screen:
						return glm::inverse(screenToClip_);
				}
				break;
			case Space::Screen:
				switch (to) {
					case Space::Camera:
						return glm::inverse(cameraToClip_) * screenToClip_;
					case Space::Clip:
						return screenToClip_;
					case Space::World:
						return glm::inverse(worldToCamera_) * glm::inverse(cameraToClip_) * screenToClip_;
					case Space::Screen:
						return glm::mat4{1};
				}
				break;
		}
		return glm::mat4{1};
	}

	void ZombieGame::zombieGameInit() {
		keyboard_ = std::make_shared<InputKeyboard>(createDefaultKeyboardKeys());

		if (Configuration::getInstance().isMusicOn()) {
			//music_ = Configuration::getInstance().getMusicTrack();
			//music_.setVolume(Configuration::getInstance().getMusicVolume());
			//music_.play(-1);
		}

		worldToCamera_ = glm::mat4{1};
		cameraToClip_ = glm::ortho(-10.f, 10.f, -10.f, 10.f);
		keyboard_ = std::make_shared<InputKeyboard>(createDefaultKeyboardKeys());

		auto humanProperties = Configuration::getInstance().getHumanProperties();
		players_.push_back(factory::createHumanPlayer(engine_, humanProperties, keyboard_));
	}

	void ZombieGame::setSize(int width, int height) {
		viewport_ = SDL_GPUViewport{
			.w = static_cast<float>(width),
			.h = static_cast<float>(height),
			.min_depth = -100.f,
			.max_depth = 100.f,
		};
		const float x = static_cast<float>(0.f);
		const float y = static_cast<float>(0.f);
		const float w = static_cast<float>(width);
		const float h = static_cast<float>(height);
		const float H = static_cast<float>(height);
		const float aspect = w / h;

		screenToClip_ = glm::ortho(x, x + w, H - y, H - y - h);

		const float delta = 10.f;
		cameraToClip_ = glm::ortho(-delta * aspect, delta * aspect, -delta, delta);
	}

	void ZombieGame::draw(Graphic& graphic, double deltaTime) {
		if (settings_.b2DebugDraw.context == nullptr) {
			settings_.b2DebugDraw = initb2DebugDraw(graphic);
		}

		updateGame(deltaTime);

		graphic.clear();
		if (!settings_.debug) {
			graphic.addRectangle({-1, -1}, {2, 2}, sdl::color::html::OliveDrab);
		}
		graphic.setMatrix(cameraToClip_ * worldToCamera_);

		if (settings_.debug) {
			engine_.debugDraw();
		} else {
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
			drawDebugArrow_.draw(graphic);
		}
		// Add this test rectangle to see if basic rendering works
		//graphic.addRectangle({-50, -50}, {100, 100}, sdl::color::html::Red);
	}

	void ZombieGame::imGuiUpdate(const sdl::DeltaTime& deltaTime) {
		ImGui::Window("Factory", [&]() {
			if (ImGui::Button("Create Car")) {
				players_.push_back(factory::createCarPlayer(engine_, CarProperties{}, keyboard_, drawDebugArrow_.position));
			}

			if (ImGui::Button("Create Zombie")) {
				players_.push_back(factory::createZombiePlayer(engine_, UnitProperties{}, drawDebugArrow_.position));
			}

			if (ImGui::Button("Create 100 Zombies")) {
				for (int i = 0; i < 100; ++i) {
					b2Vec2 delta{random(0,5), random(0,5)};
					players_.push_back(factory::createZombiePlayer(engine_, UnitProperties{}, drawDebugArrow_.position + delta));
				}
			}

			if (ImGui::Button("Clear Zombies")) {
				for (auto& player : players_) {
					if (auto ob = player->getPhysicalObject(); ob) {
						engine_.remove(*ob);
					}
				}
				players_.clear();
			}
		});

		ImGui::Window("Units", [&]() {
			ImGui::Text("Active units %d", static_cast<int>(players_.size()));
			ImGui::Text("Arrow position: (%4.2f, %4.2f)", drawDebugArrow_.position.x, drawDebugArrow_.position.y);
		});

		imguiDebugTools(settings_);
	}

	void ZombieGame::updateGame(double deltaTime) {
		timeStep_ = 1.0 / settings_.hertz;
		
		if (settings_.pause) {
			if (settings_.singleStep) {
				makeGameStep();
				settings_.singleStep = false;
			}
			return;
		}

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
		engine_.update(timeStep_, settings_.subStepCount);

		auto time = engine_.getTime();
		for (auto& player : players_) {
			player->updateInput(time, timeStep_);
		}
	}

	void ZombieGame::zoom(float scale) {
		worldToCamera_ = glm::scale(worldToCamera_, {scale, scale, 1});
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
