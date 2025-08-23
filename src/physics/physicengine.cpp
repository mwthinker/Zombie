#include "physicengine.h"
#include "gameinterface.h"
#include "physics/moving/unit.h"
#include "physics/moving/car.h"
#include "physics/weapons/missile.h"
#include "physics/weapons/weaponitem.h"

namespace zombie {

	namespace {

		void collision(GameInterface& game, float impulse, PhysicalObject* ob) {
			if (auto car = dynamic_cast<Car*>(ob)) {
				game.collision(impulse, *car);
			} else if (auto unit = dynamic_cast<Unit*>(ob)) {
				game.collision(impulse, *unit);
			}
		}

		bool getVisibleObject(MovingObject*& target, MovingObject*& looker) {
			/*
			auto fixtureA = contact->GetFixtureA();
			auto fixtureB = contact->GetFixtureB();

			// Make sure only one of the fixtures was a sensor.
			if (fixtureA->IsSensor() == fixtureB->IsSensor()) {
				return false;
			}

			auto mObA = dynamic_cast<MovingObject*>(contact->GetFixtureA()->GetUserData().physicalObject);
			auto mObB = dynamic_cast<MovingObject*>(contact->GetFixtureB()->GetUserData().physicalObject);

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
			*/
			return false;
		}
		
	}

	PhysicEngine::PhysicEngine(GameInterface& gameInterface, float impulseThreshold)
		: gameInterface_{gameInterface}
		, impulseThreshold_{impulseThreshold} {
		
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{0, 0};
		worldId_ = b2CreateWorld(&worldDef);
	}

	PhysicEngine::~PhysicEngine() {
		//world_.SetAllowSleeping(settings.m_enableSleep);
		//world_.SetWarmStarting(settings.m_enableWarmStarting);
		//world_.SetContinuousPhysics(settings.m_enableContinuous);
		//world_.SetSubStepping(settings.m_enableSubStepping);
	}

	void PhysicEngine::debugDraw() {
		b2World_Draw(worldId_, &debugDraw_);
	}

	void PhysicEngine::initDebugDraw(const b2DebugDraw& debugDraw) {
		debugDraw_ = debugDraw;
	}

	void PhysicEngine::update(double timeStep, int subStepCount) {
		// Update the objects physics interactions.
		b2World_Step(worldId_, static_cast<float>(timeStep), subStepCount);

		// Move the time ahead.
		time_ += timeStep;

		b2ContactEvents events = b2World_GetContactEvents(worldId_);

		for (int i = 0; i < events.hitCount; ++i) {
			const b2ContactHitEvent* hit = &events.hitEvents[i];

			// Get your user data back from shapes
			MovingObject* ob1 = static_cast<MovingObject*>(b2Shape_GetUserData(hit->shapeIdA));
			MovingObject* ob2 = static_cast<MovingObject*>(b2Shape_GetUserData(hit->shapeIdB));

			if (hit->approachSpeed > 1.f) {
				if (ob1) ob1->collision(hit->approachSpeed);
				if (ob2) ob2->collision(hit->approachSpeed);
			}
		}

		for (int i = 0; i < events.beginCount; ++i) {
			const b2ContactBeginTouchEvent* begin = &events.beginEvents[i];
			MovingObject* ob1 = static_cast<MovingObject*>(b2Shape_GetUserData(begin->shapeIdA));
			MovingObject* ob2 = static_cast<MovingObject*>(b2Shape_GetUserData(begin->shapeIdB));

			if (ob1 && ob2) {
				// TODO! Make objects visible to each other.
			}
		}
	}

	void PhysicEngine::add(PhysicalObject& object) {
		object.createBody(worldId_);
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
		// Todo
	}

	void PhysicEngine::doAction(Car* car) {
		// Todo
	}

}
