#ifndef ZOMBIE_PLAYER_H
#define ZOMBIE_PLAYER_H

#include "input.h"
#include "graphic.h"

namespace zombie {

	class PhysicalObject;

	class Player {
	public:
		virtual ~Player() = default;
		virtual void updateInput(double time, double deltaTime) = 0;

		virtual void draw(Graphic& graphic) = 0;

		virtual PhysicalObject* getPhysicalObject() = 0;
	};

}

#endif
