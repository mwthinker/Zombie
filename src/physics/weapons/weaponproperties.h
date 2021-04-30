#ifndef ZOMBIE_WEAPONPROPERTIES_H
#define ZOMBIE_WEAPONPROPERTIES_H

#include "box2ddef.h"
#include "missileproperties.h"

namespace zombie {

	struct WeaponProperties {
		enum class Type {Bullet, Missile};

		Type type;
		float timeBetweenShots;
		int clipSize;
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
