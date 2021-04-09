#ifndef ZOMBIE_UNITPROPERTIES_H
#define ZOMBIE_UNITPROPERTIES_H

#include "box2ddef.h"
#include "auxiliary.h"
#include "weapons/weapon.h"
#include "weapons/weaponproperties.h"

#include <sdl/opengl.h>
#include <sdl/color.h>
#include <sdl/sound.h>
#include <sdl/sprite.h>

#include <vector>

namespace zombie {

	struct UnitProperties {
		float mass = 1;
		float radius = 0.2f;
		float life = 100.f;
		float walkingSpeed = 2.f;
		float runningSpeed = 4.f;
		float stamina = 1;
		bool infected = false;
		Position grip{};

		WeaponProperties weaponProperties;
	};

}

#endif
