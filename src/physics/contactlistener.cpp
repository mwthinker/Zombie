#include "contactlistener.h"
#include "box2ddef.h"
#include "physics/moving/car.h"
#include "physics/moving/unit.h"
#include "physics/Stationary/building.h"

#include <sdl/opengl.h>

namespace zombie {

	namespace {

		void collision(GameInterface& game, float impulse, PhysicalObject* ob) {
			if (Car* car = dynamic_cast<Car*>(ob)) {
				game.collision(impulse, *car);
			} else if (Unit* unit = dynamic_cast<Unit*>(ob)) {
				game.collision(impulse, *unit);
			} else if (Building* building = dynamic_cast<Building*>(ob)) {
				game.collision(impulse, *building);
			}
		}

		bool getVisibleObject(b2Contact* contact, MovingObject*& target, MovingObject*& looker) {
			auto fixtureA = contact->GetFixtureA();
			auto fixtureB = contact->GetFixtureB();

			// Make sure only one of the fixtures was a sensor.
			bool sensorA = fixtureA->IsSensor();
			bool sensorB = fixtureB->IsSensor();
			if (sensorA == sensorB) {
				return false;
			}

			auto obA = static_cast<PhysicalObject*>(nullptr);// fixtureA->GetBody()->GetUserData());
			auto obB = static_cast<PhysicalObject*>(nullptr);//(fixtureB->GetBody()->GetUserData());
			auto mObA = dynamic_cast<MovingObject*>(obA);
			auto mObB = dynamic_cast<MovingObject*>(obB);

			// Make sure both are moving objects.
			if (mObA != nullptr && mObB != nullptr && mObA && mObB) {
				if (sensorA) {
					looker = mObA;
					target = mObB;
				} else {
					looker = mObB;
					target = mObA;
				}
				return true;
			}
			return false;
		}

	} // Anonymous namespace.

	ContactListener::ContactListener(GameInterface& gameInterface, float impulseThreshold)
		: gameInterface_{gameInterface}
		, impulseThreshold_{impulseThreshold} {
	}

	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
		auto ob1 = static_cast<PhysicalObject*>(nullptr);//contact->GetFixtureA()->GetUserData());
		auto ob2 = static_cast<PhysicalObject*>(nullptr);//contact->GetFixtureB()->GetUserData());
		float maxImpulse = 0;
		for (int32 i = 0; i < impulse->count; ++i) {
			maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
		}

		if (maxImpulse > impulseThreshold_) {
			ob1->collision(maxImpulse);
			ob2->collision(maxImpulse);
			collision(gameInterface_, maxImpulse, ob1);
			collision(gameInterface_, maxImpulse, ob2);
		}
	}

	void ContactListener::BeginContact(b2Contact* contact) {
		MovingObject* object;
		MovingObject* looker;
		if (getVisibleObject(contact, object, looker)) {
			looker->addSeenObject(object);
		}
	}

	void ContactListener::EndContact(b2Contact* contact) {
		MovingObject* object;
		MovingObject* looker;
		if (getVisibleObject(contact, object, looker)) {
			looker->removeSeenObject(object);
		}
	}

}
