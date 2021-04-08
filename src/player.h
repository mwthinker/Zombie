#ifndef ZOMBIE_PLAYER_H
#define ZOMBIE_PLAYER_H

#include "input.h"

#include <memory>

#include <sdl/graphic.h>

namespace zombie {

	class Player {
	public:
		virtual ~Player() = default;
		virtual void updateInput(double time, double deltaTime) = 0;

		virtual void draw(sdl::Graphic& graphic) = 0;
	};

	using PlayerPtr = std::shared_ptr<Player>;

}

#endif
