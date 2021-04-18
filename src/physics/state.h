#ifndef ZOMBIE_STATE_H
#define ZOMBIE_STATE_H

#include "box2ddef.h"

namespace zombie {

	struct State {
		Position position_;
		Velocity velocity_;
		float angle_;
		float anglularVelocity_;
	};

}

#endif
