#ifndef ZOMBIE_SHOT_H
#define ZOMBIE_SHOT_H

#include "box2ddef.h"
#include "auxiliary.h"

#include <sdl/opengl.h>

namespace zombie {

	class Shot {
	public:
		Shot(Position startPoint, Position endPoint)
			: position_{startPoint}
			, direction_{endPoint - startPoint} {

			float dist = direction_.Normalize();
			duration_ = dist / speed_;
			angle_ = calculateAnglePointToPoint(startPoint, endPoint) * 180 / Pi; // RAD -> DEG.
		}

		bool toBeRemoved() const {
			return time_ > duration_;
		}

	private:
		float time_{0.f};
		Position position_;
		Position direction_;
		float angle_;
		float duration_;
		float speed_{25.f};
	};

}

#endif
