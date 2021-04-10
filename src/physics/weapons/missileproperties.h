#ifndef ZOMBIE_MISSILEPROPERTIES_H
#define ZOMBIE_MISSILEPROPERTIES_H

#include <sdl/sound.h>

#include <string>

namespace zombie {

	struct MissileProperties {
		std::string name;
		float mass;
		float width;
		float length;
		sdl::Sound moveSound;
	};

}

#endif
