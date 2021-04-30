#ifndef ZOMBIE_UNITPROPERTIES_H
#define ZOMBIE_UNITPROPERTIES_H

namespace zombie {

	struct UnitProperties {
		float mass = 1;
		float radius = 0.2f;
		float life = 100.f;
		float walkingSpeed = 2.f;
		float runningSpeed = 4.f;
		float stamina = 1;
		bool infected = false;
	};

}

#endif
