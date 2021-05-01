#include "contactlistener.h"
#include "box2ddef.h"
#include "physics/moving/car.h"
#include "physics/moving/unit.h"

#include <sdl/opengl.h>

namespace zombie {

	namespace {

		void collision(GameInterface& game, float impulse, PhysicalObject* ob) {
			if (auto car = dynamic_cast<Car*>(ob)) {
				game.collision(impulse, *car);
			} else if (auto unit = dynamic_cast<Unit*>(ob)) {
				game.collision(impulse, *unit);
			}
		}

		bool getVisibleObject(b2Contact* contact, MovingObject*& target, MovingObject*& looker) {
			auto fixtureA = contact->GetFixtureA();
			auto fixtureB = contact->GetFixtureB();

			// Make sure only one of the fixtures was a sensor.
			if (fixtureA->IsSensor() == fixtureB->IsSensor()) {
				return false;
			}
			
			auto mObA = castToMovingObject(contact->GetFixtureA()->GetUserData());
			auto mObB = castToMovingObject(contact->GetFixtureB()->GetUserData());

			// Make sure both are moving objects.
			if (mObA && mObB) {
				if (fixtureA->IsSensor()) {
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

	}

	ContactListener::ContactListener(GameInterface& gameInterface, float impulseThreshold)
		: gameInterface_{gameInterface}
		, impulseThreshold_{impulseThreshold} {
	}

	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
		auto ob1 = castToMovingObject(contact->GetFixtureA()->GetUserData());
		auto ob2 = castToMovingObject(contact->GetFixtureB()->GetUserData());
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
		MovingObject* object = nullptr;
		MovingObject* looker = nullptr;
		if (getVisibleObject(contact, object, looker)) {
			looker->addSeenObject(object);
		}
	}

	void ContactListener::EndContact(b2Contact* contact) {
		MovingObject* object = nullptr;
		MovingObject* looker = nullptr;
		if (getVisibleObject(contact, object, looker)) {
			looker->removeSeenObject(object);
		}
	}

}
