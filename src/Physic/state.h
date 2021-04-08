#ifndef ZOMBIE_STATE_H
#define ZOMBIE_STATE_H

#include "box2ddef.h"

namespace zombie {

	struct State {
		Position position_{0, 0};
		Velocity velocity_{0, 0};
		float angle_{0.f};
		float anglularVelocity_{0.f};
	};

}

#endif
