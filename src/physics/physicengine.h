#ifndef ZOMBIE_ZOMBIEENGINE_H
#define ZOMBIE_ZOMBIEENGINE_H

#include "box2ddef.h"
#include "actionhandler.h"
#include "physics/physicalobject.h"
#include "graphics/debugdraw.h"

namespace zombie {

	// Forward declarations.
	class GameInterface;
	class Unit;
	class Car;
	class PhysicalObject;

	// Responsible of handeling the box2d part of the simulating.
	// I.e. hides the Box2d usage.
	class PhysicEngine : public ActionHandler {
	public:
		PhysicEngine(GameInterface& gameInterface, float impulseThreshold, b2DebugDraw& b2DebugDraw);
		~PhysicEngine();
		
		void update(double timeStep, int subStepCount);

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
			b2Vec2 delta{1, 1};

			b2AABB aabb{
				.lowerBound = position - delta,
				.upperBound = position + delta
			};

			auto filter = b2DefaultQueryFilter();

			// Perform the query
			struct Data {
				Object* object;
			};
			Data data{};

			auto overlapCallback = [](b2ShapeId shapeId, void* context) -> bool {
				auto shape = b2Shape_GetUserData(shapeId);
				Data& data = *static_cast<Data*>(context);

				if (void* userData = b2Shape_GetUserData(shapeId)) {
					auto po = static_cast<PhysicalObject*>(userData);
					if (auto object = dynamic_cast<Object*>(po)) {
						data.object = object;
						return false;
					}
				}
				return true;
			};
			b2World_OverlapAABB(worldId_, aabb, b2DefaultQueryFilter(), overlapCallback, &data);
			return data.object;
		}

		void debugDraw();

	private:
		void unitEvent(Unit* unit, int eventType) override;
		void carEvent(Car* car, int eventType) override;

		void doAction(Unit* unit);
		void doAction(Car* unit);
		
		GameInterface& gameInterface_;
		b2WorldId worldId_;
		bool started_ = false;
		double time_ = 0.0;

		float impulseThreshold_ = 0.f;

		b2DebugDraw& debugDraw_;
	};

}

#endif
