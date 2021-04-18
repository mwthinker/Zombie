#include "zombieplayer.h"
#include "auxiliary.h"
#include "physics/moving/unit.h"

#include <sdl/graphic.h>

#include <list>
#include <cassert>

namespace zombie {

	ZombiePlayer::ZombiePlayer(std::unique_ptr<Unit> unit)
		: findNewTargetTime_{random() * 3}
		, timeToUpdateAngleDirection_{random() * 1}
		, targetAngle_{unit->getDirection()}
		, unit_{std::move(unit)} {
	}

	void ZombiePlayer::updateInput(double time, double deltaTime) {
		Input input{};

		if (time > findNewTargetTime_) {
			findNewTargetTime_ = random() * 3 + static_cast<float>(time);

			target_ = findUninfectedTarget(unit_->getPosition(), unit_->getVisibleObjects());
		}

		if (time > timeToUpdateAngleDirection_) {
			timeToUpdateAngleDirection_ = random() * 1 + static_cast<float>(time);

			// Has a target?
			if (target_ != nullptr && target_->isEnabled() && !target_->isDead()) {
				auto dir = target_->getPosition() - unit_->getPosition();
				targetAngle_ = std::atan2(dir.y, dir.x);
				forward_ = true;

				double distSquared = dir.LengthSquared();
				// Target is in range?
				if (distSquared < unit_->getWeapon()->getRange() * unit_->getWeapon()->getRange()) {
					// Attack!
					input.shoot = true;
				}
			} else {
				targetAngle_ +=  (random() - 0.5f) * 2 * Pi * 2 * 0.05f;
				forward_ = random() > 0.25;
			}
		}

		double diffAngle = calculateDifferenceBetweenAngles(targetAngle_, unit_->getDirection());

		// Turn?
		if (std::abs(diffAngle) > 0.1) {
			if (diffAngle > 0) {
				input.turnLeft = true;
			} else {
				input.turnRight = true;
			}
		}

		input.forward = forward_;
		unit_->setInput(input);
		unit_->updatePhysics(time, deltaTime);
	}

	void ZombiePlayer::draw(sdl::Graphic& graphic) {
		auto pos = unit_->getPosition();
		graphic.addCircle({pos.x, pos.y}, unit_->getRadius(), sdl::color::html::Firebrick);
		graphic.addCircleOutline({pos.x, pos.y}, unit_->getViewDistance(), 0.1f, sdl::color::html::Firebrick);
	}

	MovingObject* ZombiePlayer::getMovingObject() const {
		return unit_.get();;
	}

	MovingObject* ZombiePlayer::findUninfectedTarget(Position position, const std::list<MovingObject*>& units) const {
		MovingObject* target = nullptr;

		float distant = 100;
		for (auto unit : units) {
			// Not infected?
			if (!unit->isInfected()) {
				float tmp = (position - unit->getPosition()).LengthSquared();
				// Closer?
				if (tmp < distant) {
					target = unit;
					distant = tmp;
				}
			}
		}

		return target;
	}

}
