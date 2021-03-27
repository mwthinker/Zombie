#ifndef ZOMBIE_ZOMBIEENGINE_H
#define ZOMBIE_ZOMBIEENGINE_H

#include "box2ddef.h"
#include "Physic/contactlistener.h"
#include "actionhandler.h"

namespace zombie {

	// Forward declarations.
	class Weapon;
	class GameInterface;
	class Missile;
	class Building;
	class Unit;
	class Car;
	class PhysicalObject;

	// Responsible of handeling the box2d part of the simulating.
	// I.e. hides the Box2d usage.
	class PhysicEngine : public ActionHandler {
	public:
		PhysicEngine(GameInterface& gameInterface, float impulseThreshold);
		~PhysicEngine();
		
		void update(double timeStep);
		
		void add(Unit* unit);
		
		void add(Car* car);

		// Add a generic object to the engine.
		void add(PhysicalObject* object);
		
		// Remove the object from the world.
		void remove(PhysicalObject* object);

		// Get the current game time.
		inline double getTime() const {
			return time_;
		}

		inline float getImpulseThreshold() const {
			return impulseThreshold_;
		}

	private:
		void unitEvent(Unit* unit, int eventType) override;
		void carEvent(Car* car, int eventType) override;

		void doAction(Unit* unit);
		void doAction(Car* unit);
		
		GameInterface& gameInterface_;
		ContactListener contactListener_;
		b2World world_;
		bool started_{};
		double time_{};

		float impulseThreshold_{};
	};

}

#endif
