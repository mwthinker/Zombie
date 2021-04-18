#ifndef ZOMBIE_DRAWABLE_H
#define ZOMBIE_DRAWABLE_H

#include "input.h"

#include "box2ddef.h"
#include <sdl/graphic.h>

namespace zombie {

	class DrawDebugCircle {
	public:
		Position position = Zero;

		void draw(sdl::Graphic& graphic) {
			auto start = glm::vec2{position.x, position.y - 10.f};
			auto end = glm::vec2{position.x, position.y};
			auto color = sdl::color::html::BlanchedAlmond;

			graphic.addLine(start, end, 0.2f, color);
			graphic.addLine(end + glm::vec2{-1.f, -1.f}, end, 0.2f, color);
			graphic.addLine(end + glm::vec2{1.f, -1.f}, end, 0.2f, color);
		}
		
	};

}

#endif
