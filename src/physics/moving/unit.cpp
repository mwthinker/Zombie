#include "unit.h"
#include "input.h"
#include "physics/weapons/weapon.h"
#include "auxiliary.h"

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

	void Unit::createBody(b2World& world) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Zero;
		bodyDef.angle = 0;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

		body_ = world.CreateBody(&bodyDef);
		{ // Add tensor to make all objects inside the tenson visible.
			b2CircleShape circle;
			circle.m_p.Set(0, 0);
			circle.m_radius = viewDistance_;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circle;
			fixtureDef.density = 0.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = true;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			
			body_->CreateFixture(&fixtureDef);
		}
		
		{ // Add body properties.
			b2CircleShape circle;
			circle.m_p.Set(0, 0);
			circle.m_radius = properties_.radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circle;
			fixtureDef.density = properties_.mass / (Pi * properties_.radius * properties_.radius);
			fixtureDef.friction = 0.0f;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			
			body_->CreateFixture(&fixtureDef);
		}
	}

	Unit::~Unit() {
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
			body_->ApplyForceToCenter(b2Vec2{move.x, move.y}, true);
			unitEventHandler(UnitEvent::Walk);
		} else if (!input.forward && input.backward) {
			body_->ApplyForceToCenter(-b2Vec2{move.x, move.y}, true);
			unitEventHandler(UnitEvent::Walk);
		} else {
			// In order to make the unit stop when not moving.
			body_->ApplyForceToCenter(-body_->GetLinearVelocity(), true);
			unitEventHandler(UnitEvent::Standstill);
		}

		// Add friction.
		body_->ApplyForceToCenter(-body_->GetLinearVelocity(), true);

		// Turn left or right.
		if (input.turnLeft && !input.turnRight) {
			body_->SetAngularVelocity(3.0f);
		} else if (!input.turnLeft && input.turnRight) {
			body_->SetAngularVelocity(-3.0f);
		} else {
			body_->SetAngularVelocity(0.0);
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

		if (weapon_) {
			weapon_->updateLaserSight(body_->GetWorld(), static_cast<float>(time), getPosition(), body_->GetAngle());
		}
	}
	
	State Unit::getState() const {
		return State{body_->GetPosition(),
					 body_->GetLinearVelocity(),
					 body_->GetAngle(),
					 body_->GetAngularVelocity()};
	}

	void Unit::setState(const State& state) {
		// Set the position and current angle.
		body_->SetTransform(state.position_ - body_->GetPosition(), state.angle_ - body_->GetAngle());
		
		// Set the velocity of the states.
		body_->SetAngularVelocity(state.anglularVelocity_);
		body_->SetLinearVelocity(body_->GetLinearVelocity());
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
		Position p = getPosition();
		return (position - getPosition()).LengthSquared() < getRadius()*getRadius();
	}

	bool Unit::isInsideViewArea(Position position) const {
		Position p = position - getPosition();
		double angle = std::atan2(p.y, p.x);
		return calculateDifferenceBetweenAngles(angle, body_->GetAngle() + viewAngle() * 0.5) < 0
			&& calculateDifferenceBetweenAngles(angle, body_->GetAngle() - viewAngle() * 0.5) > 0
			&& p.LengthSquared() < getViewDistance() * getViewDistance() || isInsideSmalViewDistance(position);
	}

	bool Unit::isInsideSmalViewDistance(Position position) const {
		return (position - getPosition()).LengthSquared() < smallViewDistance_*smallViewDistance_;
	}

	float Unit::getDirection() const {
		return body_->GetAngle();
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
		return body_->GetPosition();
	}

	float Unit::getRadius() const {
		return properties_.radius;
	}

	b2Body* Unit::getBody() const {
		return body_;
	}

}
