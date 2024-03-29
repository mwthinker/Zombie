#ifndef ZOMBIE_ZOMBIEENGINE_H
#define ZOMBIE_ZOMBIEENGINE_H

#include "box2ddef.h"
#include "physics/contactlistener.h"
#include "actionhandler.h"
#include "physics/physicalobject.h"

namespace zombie {

	class QueryCallback : public b2QueryCallback {
	public:
		explicit QueryCallback(b2World& world)
			: world_{world} {
		}

		PhysicalObject* getPhysicalObject(Position position) {
			position_ = position;
			const auto delta = Position{0.5f, 0.5f};
			const b2AABB& b2AABB{position - delta, position + delta};
			object_ = nullptr;
			world_.QueryAABB(this, b2AABB);
			return object_;
		}

		bool ReportFixture(b2Fixture* fixture) override {
			object_ = nullptr;
			if (!fixture->IsSensor() && fixture->TestPoint(position_)) {
				object_ = fixture->GetUserData().physicalObject;
			}
			return object_ == nullptr;
		}

	private:
		b2World& world_;
		PhysicalObject* object_ = nullptr;
		Position position_{};
	};

	// Forward declarations.
	class GameInterface;
	class Unit;
	class Car;
	class PhysicalObject;

	// Responsible of handeling the box2d part of the simulating.
	// I.e. hides the Box2d usage.
	class PhysicEngine : public ActionHandler {
	public:
		PhysicEngine(GameInterface& gameInterface, float impulseThreshold);
		~PhysicEngine();
		
		void update(double timeStep, int velocityIterations = 6, int positionIterations = 2);

		// Add a generic object to the engine.
		void add(PhysicalObject& object);
		
		// Remove the object from the world.
		void remove(PhysicalObject& object);

		// Get the current game time.
		inline double getTime() const {
			return time_;
		}

		inline float getImpulseThreshold() const {
			return impulseThreshold_;
		}

		template <typename Object> requires std::derived_from<Object, PhysicalObject>
		Object* query(Position position) {
			auto ob = QueryCallback{world_}.getPhysicalObject(position);
			return dynamic_cast<Object*>(ob);
		}

		void setDebugDraw(b2Draw* draw);

		void debugDraw();

	private:
		void unitEvent(Unit* unit, int eventType) override;
		void carEvent(Car* car, int eventType) override;

		void doAction(Unit* unit);
		void doAction(Car* unit);
		
		GameInterface& gameInterface_;
		ContactListener contactListener_;
		b2World world_;
		bool started_ = false;
		double time_ = 0.0;

		float impulseThreshold_ = 0.f;
	};

}

#endif
