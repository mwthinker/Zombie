#ifndef ZOMBIE_DEBUGDRAW_H
#define ZOMBIE_DEBUGDRAW_H

#include "graphics/debugdraw.h"
#include "../graphic.h"

#include <box2d/box2d.h>
#include <sdl/color.h>

#include <utility>

namespace zombie {

	b2DebugDraw initb2DebugDraw(Graphic& graphic);

}

#endif
