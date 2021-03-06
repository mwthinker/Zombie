#include "car.h"
#include "input.h"
#include "physics/state.h"
#include "unit.h"

#include <cmath>

namespace zombie {

	Car::Car(const CarProperties& properties)
		: properties_{properties} {
	}

	void Car::updatePhysics(double time, double timeStep) {
		Input input = getInput();
		bool brake = true;
		b2Vec2 force = getDirectionVector();

		// Accelerate or decelerate
		float throttle = 0.0f;
		if (input.forward && !input.backward) {
			throttle = 100.0;
		} else if (!input.forward && input.backward) {
			throttle = -20.0;
		}
		body_->ApplyForce(throttle * force, getFrontWheelPosition(), true);

		float steering = 0.0f;

		// Turn left or right
		if (input.turnLeft && !input.turnRight) {
			steering = 1.0f;
		} else if (!input.turnLeft && input.turnRight) {
			steering = -1.0f;
		}

		steeringAngle_ = 0.4f * steering;

		applyFriction(brake, 1.0f, 1.0f, 10.0f, 10.0f);

		carEventHandler(CarEvent::Moved);

		if (input.action) {
			carEventHandler(CarEvent::Action);
		}
	}

	State Car::getState() const {
		return State{body_->GetPosition(),
					 body_->GetLinearVelocity(),
					 body_->GetAngle(),
					 body_->GetAngularVelocity()};
	}

	void Car::setState(const State& state) {
		body_->SetTransform(state.position_ - body_->GetPosition(), state.angle_ - body_->GetAngle());
		body_->SetAngularVelocity(state.anglularVelocity_);
		body_->SetLinearVelocity(body_->GetLinearVelocity());
	}

	bool Car::isInsideViewArea(Position position) const {
		return true;
	}

	void Car::createBody(b2World& world) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Zero;
		bodyDef.angle = 0;
		bodyDef.userData.physicalObject = this;
		
		body_ = world.CreateBody(&bodyDef);
		{
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(0.5f * properties_.length, 0.5f * properties_.width); // Expected parameters is half the side.

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = properties_.mass / (properties_.length * properties_.width);
			fixtureDef.friction = 0.3f;
			fixtureDef.userData.physicalObject = this;
			body_->CreateFixture(&fixtureDef);
		}
	}

	void Car::applyFriction(bool brake, float frictionForwardFrontWheel, float frictionForwardBackWheel,
		float frictionLateralFrontWheel, float frictionLateralBackWheel) {
				
		// Back wheel lateral friction.
		float friction = frictionLateralBackWheel;
		b2Vec2 currentRightNormal = body_->GetWorldVector(b2Vec2(0, -1));
		b2Vec2 force = -friction * b2Dot(currentRightNormal, body_->GetLinearVelocityFromWorldPoint(getBackWheelPosition())) * currentRightNormal;
		body_->ApplyForce(force, getBackWheelPosition(), true);
				
		// Front wheel lateral friction.
		friction = frictionLateralFrontWheel;
		currentRightNormal = b2Vec2(-getDirectionVector().y, getDirectionVector().x);
		force = -friction * b2Dot(currentRightNormal, body_->GetLinearVelocityFromWorldPoint(getFrontWheelPosition())) * currentRightNormal;
		body_->ApplyForce(force, getFrontWheelPosition(), true);
				
		// Back wheel forward friction.
		friction = frictionForwardBackWheel;
		if (brake) {
			friction = frictionLateralBackWheel;
		}
		force = -friction * b2Dot(getDirectionVector(), body_->GetLinearVelocity()) * getDirectionVector();
		body_->ApplyForce(force, getBackWheelPosition(), true);
				
		// Front wheel forward friction.
		friction = frictionForwardFrontWheel;
		if (brake) {
			friction = frictionLateralFrontWheel;
		}
		b2Vec2 forward = body_->GetWorldVector(b2Vec2(1, 0));
		force = -friction * b2Dot(forward, body_->GetLinearVelocity()) * forward;
		body_->ApplyForce(force, getFrontWheelPosition(), true);
	}

}
