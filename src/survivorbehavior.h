#ifndef ZOMBIE_SURVIVORBEHAVIOR_H
#define ZOMBIE_SURVIVORBEHAVIOR_H

#include "input.h"
#include "box2ddef.h"
#include "Physic/Moving/unit.h"
#include "player.h"

#include <vector>

namespace zombie {

	class SurvivorBehavior : public Player {
	public:
		SurvivorBehavior(Unit* unit);
		~SurvivorBehavior();
		
		void updateInput(double time, double deltaTime) override;

		MovingObject* getMovingObject() const;

	private:
		Unit* findUninfectedTarget(Position position, const std::vector<Unit*>& units) const;

		double findNewTargetTime_;
		Unit* target_{};
		Unit* unit_;
	};

}

#endif
