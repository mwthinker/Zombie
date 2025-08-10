#ifndef ZOMBIE_DRAWDEBUGARROW_H
#define ZOMBIE_DRAWDEBUGARROW_H

#include "input.h"
#include "box2ddef.h"
#include "../graphic.h"

namespace zombie {

	class DrawDebugArrow {
	public:
		Position position = Zero;

		void draw(Graphic& graphic) {
			auto start = glm::vec2{position.x, position.y - 10.f};
			auto end = glm::vec2{position.x, position.y};
			static constexpr auto color = sdl::color::html::BlanchedAlmond;

			graphic.addLine(start, end, 0.2f, color);
			graphic.addLine(end + glm::vec2{-1.f, -1.f}, end, 0.2f, color);
			graphic.addLine(end + glm::vec2{1.f, -1.f}, end, 0.2f, color);
		}
		
	};

}

#endif
