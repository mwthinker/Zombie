#include "physicengine.h"
#include "gameinterface.h"
#include "physics/moving/unit.h"
#include "physics/moving/car.h"
#include "physics/weapons/missile.h"
#include "physics/weapons/weaponitem.h"
#include "physics/closestraycastcallback.h"

namespace zombie {

	namespace {

		
	}

	PhysicEngine::PhysicEngine(GameInterface& gameInterface, float impulseThreshold)
		: gameInterface_{gameInterface}
		, contactListener_{gameInterface, impulseThreshold}
		, world_{b2Vec2{0, 0}} {
		
		world_.SetContactListener(&contactListener_);
	}

	PhysicEngine::~PhysicEngine() {
	}

	void PhysicEngine::update(double timeStep) {
		// Update the objects physics interactions.
		world_.Step(static_cast<float>(timeStep), 6, 2);

		// Move the time ahead.
		time_ += timeStep;
	}

	void PhysicEngine::add(PhysicalObject& object) {
		object.createBody(world_);
		object.setEnabled(false);
		object.setAwake(false);
	}

	void PhysicEngine::remove(PhysicalObject& object) {
		object.destroyBody();
	}

	void PhysicEngine::unitEvent(Unit* unit, int eventType) {
		switch (static_cast<UnitEvent>(eventType)) {
			case UnitEvent::Action:
				doAction(unit);
				break;
			case UnitEvent::Die:
				gameInterface_.unitDied(*unit);
				break;
			default:
				break;
		}
	}

	void PhysicEngine::carEvent(Car* car, int eventType) {
		switch (static_cast<CarEvent>(eventType)) {
			case CarEvent::Action:
				doAction(car);
				break;
			default:
				break;
		}
	}

	void PhysicEngine::doAction(Unit* unit) {
		float angle = unit->getDirection();
		b2Vec2 dir{std::cos(angle), std::sin(angle)};

		// Return the closest object, physical or not.
		ClosestRayCastCallback callback([](b2Fixture* fixture) {
			return false;// !fixture->IsSensor() && fixture->GetBody()->GetUserData() != nullptr || fixture->IsSensor() && fixture->GetUserData() != nullptr;
		});
		world_.RayCast(&callback, unit->getPosition(), unit->getPosition() + dir);
		auto fixture = callback.getFixture();

		// Is there an object near by?
		if (fixture != nullptr) {
			// Safe to do, because all userdata must be a PhysicalObject!
			auto ob = fixture->GetUserData().physicalObject;

			//if (auto car = std::dynamic_pointer_cast<Car>(ob)) {
				// ToDo
			//} else if (auto wItem = std::dynamic_pointer_cast<WeaponItem>(ob)) {
				// Todo
			//}
		}
	}

	void PhysicEngine::doAction(Car* car) {
		// Todo
	}

}
