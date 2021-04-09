#include "physicengine.h"
#include "gameinterface.h"
#include "physics/moving/unit.h"
#include "physics/moving/car.h"
#include "weapons/missile.h"
#include "physics/Stationary/building.h"
#include "weapons/weaponitem.h"
#include "physics/closestraycastcallback.h"

namespace zombie {

	PhysicEngine::PhysicEngine(GameInterface& gameInterface, float impulseThreshold)
		: gameInterface_{gameInterface}
		, world_{b2Vec2{0, 0}}
		, contactListener_{gameInterface, impulseThreshold} {
		
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

	void PhysicEngine::add(Unit* unit) {
		assert(dynamic_cast<PhysicalObject*>(unit) != nullptr); // Test, PhysicalObject type and not null.
		(static_cast<PhysicalObject*>(unit))->createBody(world_);
		unit->setEnabled(false);
		unit->setAwake(false);
		//unit->getWeapon()->init(&world_, &gameInterface_);
	}

	void PhysicEngine::add(Car* car) {
		assert(dynamic_cast<PhysicalObject*>(car) != nullptr); // Test, PhysicalObject type and not null.
		(static_cast<PhysicalObject*>(car))->createBody(world_);
		car->setEnabled(false);
		car->setAwake(false);
	}

	void PhysicEngine::add(PhysicalObject* object) {
		assert(object != nullptr);
		object->createBody(world_);
		object->setEnabled(false);
		object->setAwake(false);
	}

	void PhysicEngine::remove(PhysicalObject* object) {
		assert(object != nullptr);
		object->destroyBody();
	}

	void PhysicEngine::unitEvent(Unit* unit, int eventType) {
		switch (static_cast<UnitEvent>(eventType)) {
			case UnitEvent::Action:
				doAction(unit);
				break;
			case UnitEvent::Die:
				gameInterface_.unitDied(*unit);
				break;
		}
	}

	void PhysicEngine::carEvent(Car* car, int eventType) {
		switch (static_cast<CarEvent>(eventType)) {
			case CarEvent::Action:
				doAction(car);
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
			auto ob = static_cast<PhysicalObject*>(nullptr); // (fixture->GetUserData());

			if (auto car = dynamic_cast<Car*>(ob)) {
				// Car is empty?
				if (car->getDriver() == nullptr) {
					// The player is able to drive?
					car->setDriver(unit);
					unit->setEnabled(false);
				}
			} else if (auto wItem = dynamic_cast<WeaponItem*>(ob)) {
				// Change the weapon.
				unit->setWeapon(wItem->getWeapon());
				//remove(wItem);
			}
		}
	}

	void PhysicEngine::doAction(Car* car) {
		auto driver = car->getDriver();
		driver->setEnabled(true);
		car->setDriver(nullptr);
	}

}
