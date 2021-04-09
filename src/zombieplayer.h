#ifndef ZOMBIE_ZOMBIEPLAYER_H
#define ZOMBIE_ZOMBIEPLAYER_H

#include "box2ddef.h"
#include "input.h"
#include "player.h"

#include <list>

namespace zombie {

	class Unit;
	class MovingObject;

	class ZombiePlayer : public Player {
	public:
		ZombiePlayer(std::unique_ptr<Unit> unit);

		void updateInput(double time, double deltaTime) override;
		void draw(sdl::Graphic& graphic) override;

		MovingObject* getMovingObject() const;

	private:
		MovingObject* findUninfectedTarget(Position position, const std::list<MovingObject*>& units) const;

		float findNewTargetTime_ = 0.f;
		float timeToUpdateAngleDirection_ = 0.f;
		float targetAngle_ = 0.f;
		bool forward_ = false;

		std::unique_ptr<Unit> unit_;
		MovingObject* target_{};
	};

	using ZombiePlayerPtr = std::unique_ptr<ZombiePlayer>;

}

#endif
