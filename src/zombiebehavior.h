#ifndef ZOMBIE_ZOMBIEBEHAVIOR_H
#define ZOMBIE_ZOMBIEBEHAVIOR_H

#include "box2ddef.h"
#include "input.h"
#include "player.h"

#include <list>

namespace zombie {

	class Unit;
	class MovingObject;

	class ZombieBehavior : public Player {
	public:
		ZombieBehavior(Unit* unit);
		~ZombieBehavior();

		void updateInput(double time, double deltaTime) override;

		MovingObject* getMovingObject() const;

	private:
		MovingObject* findUninfectedTarget(Position position, const std::list<MovingObject*>& units) const;

		float findNewTargetTime_;
		float timeToUpdateAngleDirection_;
		float targetAngle_;
		bool forward_;

		Unit* unit_;
		MovingObject* target_{};
	};

}

#endif
