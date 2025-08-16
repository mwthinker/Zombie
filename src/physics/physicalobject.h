#ifndef ZOMBIE_OBJECT_H
#define ZOMBIE_OBJECT_H

#include "box2ddef.h"

#include <cassert>
#include <memory>

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
		virtual b2BodyId getBody() const = 0;

		void setEnabled(bool active) {
			if (active) {
				b2Body_Enable(getBody());
			} else {
				b2Body_Disable(getBody());
			}
		}
		
		bool isEnabled() const {
			return b2Body_IsEnabled(getBody());
		}

		void setAwake(bool awake) {
			b2Body_SetAwake(getBody(), awake);
		}

		bool isAwake() const {
			return b2Body_IsAwake(getBody());
		}

	private:
		// Functions should only be called by the class PhysicEngine.
		friend class PhysicEngine;

		// Create the body. I.e. the body is now a part of the box2d world.
		virtual void createBody(b2WorldId world) = 0;

		// Destroys the body. I.e. the body in no more a part of the box2d world.
		virtual void destroyBody() = 0;
	};

}

#endif
