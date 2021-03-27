#ifndef ZOMBIE_PLAYER_H
#define ZOMBIE_PLAYER_H

#include "input.h"

namespace zombie {

	class Player {
	public:
		virtual ~Player() = default;
		virtual void updateInput(double time, double deltaTime) = 0;
	};

}

#endif
