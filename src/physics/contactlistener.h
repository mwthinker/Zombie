#ifndef ZOMBIE_CONTACTLISTENER_H
#define ZOMBIE_CONTACTLISTENER_H

#include "box2ddef.h"
#include "gameinterface.h"

namespace zombie {

	class ContactListener : public b2ContactListener {
	public:
		ContactListener(GameInterface& gameInterface, float impulseThreshold);

		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

		void BeginContact(b2Contact* contact) override;

		// Is called when the contact has get out of view or is destroyed.
		void EndContact(b2Contact* contact) override;

	private:
		GameInterface& gameInterface_;
		float impulseThreshold_;
	};

}

#endif
