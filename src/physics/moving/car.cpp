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
		b2Body_ApplyForce(bodyId_, throttle * force, getBackWheelPosition(), true);

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
		return State{
			getPosition(),
			b2Body_GetLinearVelocity(bodyId_),
			b2Rot_GetAngle(b2Body_GetRotation(bodyId_)),
			b2Body_GetAngularVelocity(bodyId_)
		};
	}

	void Car::setState(const State& state) {
		b2Body_SetTransform(bodyId_, state.position_ - getPosition(), b2MakeRot(state.angle_ - getDirection()));
		b2Body_SetAngularVelocity(bodyId_, (state.anglularVelocity_));
		b2Body_SetLinearVelocity(bodyId_, b2Body_GetLinearVelocity(bodyId_));
	}

	bool Car::isInsideViewArea(Position position) const {
		return true;
	}

	void Car::createBody(b2WorldId world) {
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Zero;
		bodyDef.rotation = b2MakeRot(0);
		bodyDef.userData = this;

		bodyId_ = b2CreateBody(world, &bodyDef);
		{
			b2Polygon dynamicBox = b2MakeBox(0.5f * properties_.length, 0.5f * properties_.width);

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = properties_.mass / (properties_.length * properties_.width);
			//shapeDef.friction = 0.3f;
			shapeDef.userData = this;

			b2CreatePolygonShape(bodyId_, &shapeDef, &dynamicBox);
		}
	}

	void Car::applyFriction(bool brake, float frictionForwardFrontWheel, float frictionForwardBackWheel,
		float frictionLateralFrontWheel, float frictionLateralBackWheel) {
				
		// Back wheel lateral friction.
		float friction = frictionLateralBackWheel;
		b2Vec2 currentRightNormal = b2Body_GetWorldVector(bodyId_, b2Vec2{0, -1});
		
		b2Vec2 force{};// = -friction * b2Dot(currentRightNormal, body_->GetLinearVelocityFromWorldPoint(getBackWheelPosition())) * currentRightNormal;
		
		b2Body_ApplyForce(bodyId_, force, getBackWheelPosition(), true);
				
		// Front wheel lateral friction.
		friction = frictionLateralFrontWheel;
		currentRightNormal = b2Vec2(-getDirectionVector().y, getDirectionVector().x);
		//force = -friction * b2Dot(currentRightNormal, body_->GetLinearVelocityFromWorldPoint(getFrontWheelPosition())) * currentRightNormal;
		b2Body_ApplyForce(bodyId_, force, getFrontWheelPosition(), true);
				
		// Back wheel forward friction.
		friction = frictionForwardBackWheel;
		if (brake) {
			friction = frictionLateralBackWheel;
		}
		force = -friction * b2Dot(getDirectionVector(), b2Body_GetLinearVelocity(bodyId_)) * getDirectionVector();
		b2Body_ApplyForce(bodyId_, force, getBackWheelPosition(), true);
				
		// Front wheel forward friction.
		friction = frictionForwardFrontWheel;
		if (brake) {
			friction = frictionLateralFrontWheel;
		}
		b2Vec2 forward = b2Body_GetWorldVector(bodyId_, b2Vec2{1, 0});
		force = -friction * b2Dot(forward, b2Body_GetLinearVelocity(bodyId_)) * forward;
		b2Body_ApplyForce(bodyId_, force, getFrontWheelPosition(), true);
	}

}
