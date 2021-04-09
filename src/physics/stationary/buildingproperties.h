#ifndef ZOMBIE_BUILDINGPROPERTIES_H
#define ZOMBIE_BUILDINGPROPERTIES_H

//#include "animation.h"

#include <sdl/sound.h>

#include <string>

namespace zombie {

	struct MapProperties {
		enum class ObjectType {Building, Water, Grass, TilePoint, Tree, SpawningPoint, CarSpawningPoint};
		
		struct Object {
			ObjectType type_;
			std::vector<Position> geom_;
		};

		std::string name_;
		std::vector<Object> positions_;
	};

}

#endif
