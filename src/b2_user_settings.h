#ifndef ZOMBIE_B2_USER_SETTINGS_H
#define ZOMBIE_B2_USER_SETTINGS_H

#include <box2d/b2_types.h>

#include <spdlog/spdlog.h>

namespace zombie {

	class PhysicalObject;

}

constexpr float b2_lengthUnitsPerMeter = 1.0f;

constexpr int b2_maxPolygonVertices = 8;

struct b2BodyUserData {
	zombie::PhysicalObject* physicalObject = nullptr;
};

struct b2FixtureUserData {
	zombie::PhysicalObject* physicalObject = nullptr;
};

struct b2JointUserData {
	zombie::PhysicalObject* physicalObject = nullptr;
};

void* b2Alloc_Default(int32 size);
void b2Free_Default(void* mem);

inline void* b2Alloc(int32 size) {
	return b2Alloc_Default(size);
}

inline void b2Free(void* mem) {
	b2Free_Default(mem);
}

template <typename... Args>
inline void b2Log(const char* string, Args&&... args) {
	spdlog::debug(string, std::forward<Args>(args)...);
}

#include <box2d/b2_common.h>

#endif
