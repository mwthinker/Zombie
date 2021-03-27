#ifndef ZOMBIE_WEAPONPROPERTIES_H
#define ZOMBIE_WEAPONPROPERTIES_H

#include "box2ddef.h"
#include "auxiliary.h"
#include "weapon.h"
#include "missileproperties.h"

#include <sdl/opengl.h>
#include <sdl/color.h>
#include <sdl/sound.h>
#include <sdl/sprite.h>

#include <vector>
#include <string>

namespace zombie {

	struct WeaponProperties {
		enum class Type {Bullet, Missile};

		std::string name;
		Type type;
		sdl::Sprite symbolImage;
		float timeBetweenShots;
		int clipSize;
		sdl::Sound shootSound;
		sdl::Sound reloadSound;
		float size;
		Position moveImageGrip;
		MissileProperties missileProperties;

		float damageRadius;
		float damage;
		float range;
		float deathTime;
		float speed;
		float force;

		bool laserSight;
	};

}

#endif
