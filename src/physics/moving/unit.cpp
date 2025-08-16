#include "unit.h"
#include "input.h"
#include "physics/weapons/weapon.h"
#include "auxiliary.h"

#include <spdlog/spdlog.h>

#include <cmath>

namespace zombie {

	Unit::Unit(const UnitProperties& unitProperties, WeaponPtr weapon)
		: properties_{unitProperties}
		, weapon_{std::move(weapon)} {

		// Properties
		viewDistance_ = 10.f;
		viewAngle_ = 120.f / 180.f * Pi;
		smallViewDistance_ = 2;

		// Health
		isDead_ = false;
		died_ = false;

		timeLeftToRun_ = 5.f;
	}

	void Unit::createBody(b2WorldId world) {
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Zero;
		bodyDef.rotation = b2MakeRot(0);
		bodyDef.userData = this;

		bodyId_ = b2CreateBody(world, &bodyDef);
		{ // Add tensor to make all objects inside the tenson visible.
			b2Circle circle{
				.center = Zero,
				.radius = viewDistance_
			};

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = 0.0f;
			//shapeDef.friction = 0.0f;
			shapeDef.isSensor = true;
			shapeDef.userData = this;

			auto circleShapeId = b2CreateCircleShape(bodyId_, &shapeDef, &circle);
		}
		
		{ // Add body properties.
			b2Circle circle{
				.center = Zero,
				.radius = properties_.radius
			};

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = properties_.mass / (Pi * properties_.radius * properties_.radius);
			//shapeDef.friction = 0.0f;
			shapeDef.isSensor = true;
			shapeDef.userData = this;
			
			auto circleShapeId = b2CreateCircleShape(bodyId_, &shapeDef, &circle);
		}
	}

	// All UnitEvent is only allowed to be triggered in this function.
	// This in order to avoid adjusting box2d valuse during a box2d step, which
	// would cause undefined behavior.
	void Unit::updatePhysics(double time, double timeStep) {
		if (died_) {
			unitEventHandler(UnitEvent::Die);
			isDead_ = true;
		}

		if (isDead()) {
			return;
		}
		
		Input input = getInput();
		float angle = getDirection();
		Force move{std::cos(angle), std::sin(angle)};

		// Time left to run?
		if (timeLeftToRun_ >= 0) {
			if (input.forward && input.run) {
				timeLeftToRun_ -= static_cast<float>(timeStep);
				move *= 2;
				unitEventHandler(UnitEvent::Run);
			} else if (timeLeftToRun_ < 5) {
				timeLeftToRun_ += static_cast<float>(timeStep);
			}
		} else { // Time is negative!
			timeLeftToRun_ += static_cast<float>(timeStep);
		}

		// Move forward or backwards.
		if (input.forward && !input.backward) {
			b2Body_ApplyForceToCenter(bodyId_, b2Vec2{move.x, move.y}, true);
			unitEventHandler(UnitEvent::Walk);
			spdlog::info("Force: {}", b2Body_GetPosition(bodyId_));
		} else if (!input.forward && input.backward) {
			b2Body_ApplyForceToCenter(bodyId_, -b2Vec2{move.x, move.y}, true);
			unitEventHandler(UnitEvent::Walk);
		} else {
			// In order to make the unit stop when not moving.
			b2Body_ApplyForceToCenter(bodyId_, -b2Body_GetLinearVelocity(bodyId_), true);
			unitEventHandler(UnitEvent::Standstill);
		}

		// Add friction.
		//body_->ApplyForceToCenter(-body_->GetLinearVelocity(), true);

		// Turn left or right.
		if (input.turnLeft && !input.turnRight) {
			b2Body_SetAngularVelocity(bodyId_, 3.0f);
		} else if (!input.turnLeft && input.turnRight) {
			b2Body_SetAngularVelocity(bodyId_, -3.0f);
		} else {
			b2Body_SetAngularVelocity(bodyId_, 0.0f);
		}

		// Want to shoot?
		if (input.shoot && weapon_) {
			weapon_->pullTrigger(*this, static_cast<float>(time));
			weapon_->releaseTrigger(*this, static_cast<float>(time));
		}

		// Want to reload?
		if (input.reload) {
			if (weapon_) {
				weapon_->reload(static_cast<float>(time));
			}
		}

		if (input.action) {
			unitEventHandler(UnitEvent::Action);
		}
	}
	
	State Unit::getState() const {
		return State{
			b2Body_GetPosition(bodyId_),
			b2Body_GetLinearVelocity(bodyId_),
			b2Rot_GetAngle(b2Body_GetRotation(bodyId_)),
			b2Body_GetAngularVelocity(bodyId_)
		};
	}

	void Unit::setState(const State& state) {
		// Set the position and current angle.
		b2Body_SetTransform(bodyId_, state.position_ - b2Body_GetPosition(bodyId_), b2MakeRot(state.angle_ - b2Rot_GetAngle(b2Body_GetRotation(bodyId_))));
		
		// Set the velocity of the states.
		b2Body_SetAngularVelocity(bodyId_, state.anglularVelocity_);
		b2Body_SetLinearVelocity(bodyId_, state.velocity_);
	}

	float Unit::getViewDistance() const {
		return viewDistance_;
	}

	float Unit::smallViewDistance() const {
		return smallViewDistance_;
	}

	float Unit::viewAngle() const {
		return viewAngle_;
	}
	
	bool Unit::isInside(Position position) const {
		return b2LengthSquared(position - getPosition()) < getRadius()*getRadius();
	}

	bool Unit::isInsideViewArea(Position position) const {
		Position p = position - getPosition();
		auto angle = std::atan2(p.y, p.x);
		return (calculateDifferenceBetweenAngles(angle, b2Rot_GetAngle(b2Body_GetRotation(bodyId_)) + viewAngle() * 0.5f) < 0
			&& calculateDifferenceBetweenAngles(angle, b2Rot_GetAngle(b2Body_GetRotation(bodyId_)) - viewAngle() * 0.5f) > 0
			&& b2LengthSquared(p) < getViewDistance() * getViewDistance()) || isInsideSmalViewDistance(position);
	}

	bool Unit::isInsideSmalViewDistance(Position position) const {
		return b2LengthSquared((position - getPosition())) < smallViewDistance_*smallViewDistance_;
	}

	float Unit::getDirection() const {
		return b2Rot_GetAngle(b2Body_GetRotation(bodyId_));
	}

	float Unit::healthPoints() const {
		return properties_.life;
	}

	void Unit::updateHealthPoint(float deltaLife) {
		if (!isDead_) {
			properties_.life += deltaLife;
			unitEventHandler(UnitEvent::Injured);
		}
		if (properties_.life < 0) {
			died_ = true;
		}
	}

	bool Unit::isDead() const {
		return isDead_;
	}

	Position Unit::getPosition() const {
		return b2Body_GetPosition(bodyId_);
	}

	float Unit::getRadius() const {
		return properties_.radius;
	}

	b2BodyId Unit::getBody() const {
		return bodyId_;
	}

}
