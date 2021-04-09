#ifndef ZOMBIE_OBJECT_H
#define ZOMBIE_OBJECT_H

#include "box2ddef.h"

#include <cassert>

namespace zombie {

	// Represent a object in the box2d world inside "zombie world".
	class PhysicalObject {
	public:
		// Must call destroyBody() before destructor is called.
		virtual ~PhysicalObject() = default;

		// Is called when a collision occurs.
		virtual void collision(float impulse) {
		}

		// Return the body for the object. Use with care!
		// Return a nullpntr if the object not belongs to the world.
		virtual b2Body* getBody() const = 0;

		void setEnabled(bool active) {
			assert(getBody() != nullptr);
			getBody()->SetEnabled(active);
		}
		
		bool isEnabled() const {
			assert(getBody() != nullptr);
			return getBody()->IsEnabled();
		}

		void setAwake(bool awake) {
			assert(getBody() != nullptr);
			getBody()->SetAwake(awake);
		}

		bool isAwake() const {
			assert(getBody() != nullptr);
			return getBody()->IsAwake();
		}

	private:
		// Functions should only be called by the class PhysicEngine.
		friend class PhysicEngine;

		// Create the body. I.e. the body is now a part of the box2d world.
		virtual void createBody(b2World& world) = 0;

		// Destroys the body. I.e. the body in no more a part of the box2d world.
		virtual void destroyBody() = 0;
	};

}

#endif
