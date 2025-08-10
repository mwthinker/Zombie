#include "zombieplayer.h"
#include "auxiliary.h"
#include "physics/moving/unit.h"

//#include <sdl/graphic.h>

#include <list>
#include <cassert>

#include <glm/gtx/rotate_vector.hpp>

namespace zombie {

	ZombiePlayer::ZombiePlayer(std::unique_ptr<Unit> unit)
		: findNewTargetTime_{random(0.f, 3.f)}
		, timeToUpdateAngleDirection_{random()}
		, targetAngle_{unit->getDirection()}
		, unit_{std::move(unit)} {
	}

	void ZombiePlayer::updateInput(double time, double deltaTime) {
		Input input{};

		if (time > findNewTargetTime_) {
			findNewTargetTime_ = random(0.f, 3.f) + static_cast<float>(time);

			target_ = findUninfectedTarget(unit_->getPosition(), unit_->getVisibleObjects());
		}

		if (time > timeToUpdateAngleDirection_) {
			timeToUpdateAngleDirection_ = random() + static_cast<float>(time);

			// Has a target?
			if (false && target_ != nullptr && target_->isEnabled() && !target_->isDead()) {
				auto dir = target_->getPosition() - unit_->getPosition();
				targetAngle_ = std::atan2(dir.y, dir.x);
				forward_ = true;

				auto distSquared = dir.LengthSquared();
				
				// Target is in range?
				if (auto& weapon = unit_->getWeapon();
					weapon != nullptr && distSquared < weapon->getRange() * weapon->getRange()) {
					
					// Attack!
					input.shoot = true;
				}
			} else {
				targetAngle_ +=  random(-0.5f, 0.5f) * 2.f * Pi * 2.f * 0.05f;
				forward_ = random() > 0.25f;
			}
		}

		auto diffAngle = calculateDifferenceBetweenAngles(targetAngle_, unit_->getDirection());

		// Turn?
		if (std::abs(diffAngle) > 0.1f) {
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

	void ZombiePlayer::draw(Graphic& graphic) {
		auto pos = unit_->getPosition();
		graphic.addCircle({pos.x, pos.y}, unit_->getRadius(), sdl::color::html::Firebrick);
		graphic.addCircleOutline({pos.x, pos.y}, unit_->getViewDistance(), 0.04f, sdl::color::html::Firebrick);
		graphic.addLine({pos.x, pos.y}, glm::vec2{pos.x, pos.y} + glm::rotate(glm::vec2{1.f, 0.f}*0.5f, unit_->getDirection()), 0.1f, sdl::color::html::Firebrick);
	} 

	PhysicalObject* ZombiePlayer::getPhysicalObject() {
		return unit_.get();
	}

	MovingObject* ZombiePlayer::findUninfectedTarget(Position position, const std::list<MovingObject*>& units) const {
		MovingObject* target = nullptr;
		float distance = 100.f;
		for (auto unit : units) {
			if (!unit->isInfected()) {
				auto tmp = (position - unit->getPosition()).LengthSquared();
				if (tmp < distance) {
					target = unit;
					distance = tmp;
				}
			}
		}
		return target;
	}

}
