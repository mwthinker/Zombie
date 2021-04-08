#include "car.h"
#include "input.h"
#include "Physic/state.h"
#include "unit.h"

#include <cmath>

namespace zombie {

	Car::Car(float mass, float life, float width, float length)
		: length_{length}
		, width_{width}
		, mass_{mass} {
	}

	Car::Car(const Car& car)
		: length_{car.length_}
		, width_{car.width_}
		, currentTime_{car.currentTime_}
		, steeringAngle_{car.steeringAngle_}
		, wheelDelta_{car.wheelDelta_}
		, mass_{car.mass_} {
	}
	
	Car& Car::operator=(const Car& car) {
		length_ = car.length_;
		width_ = car.width_;

		currentTime_ = car.currentTime_;
		steeringAngle_ = car.steeringAngle_;
		wheelDelta_ = car.wheelDelta_;
		mass_ = car.mass_;
		body_ = nullptr;

		return  *this;
	}

	Unit* Car::getDriver() const {
		return driver_;
	}

	void Car::setDriver(Unit* driver) {
		driver_ = driver;
	}

	void Car::updatePhysics(double time, double timeStep) {
		Input input{};
		bool brake = true;
		if (driver_ != nullptr) {
			input = driver_->getInput();
			brake = false;
		}
		previousState_ = getState();
		b2Vec2 force = getDirectionVector();

		// Accelate or decelerate
		float throttle = 0.0f;
		if (input.forward && !input.backward) {
			throttle = 20.0;
		} else if (!input.forward && input.backward) {
			throttle = -20.0;
		}
		body_->ApplyForce(throttle*force, getFrontWheelPosition(), true);

		float steering = 0.0f;

		// Turn left or right
		if (input.turnLeft && !input.turnRight) {
			steering = 1.0f;
		} else if (!input.turnLeft && input.turnRight) {
			steering = -1.0f;
		}

		steeringAngle_ = 0.4f * steering;

		applyFriction(brake, 2.0f, 2.0f, 100.0f, 100.0f);

		//signal(MOVED);

		if (input.action) {
			//signal(ACTION);
		}
	}

	State Car::getState() const {
		return State{body_->GetPosition(),
					 body_->GetLinearVelocity(),
					 body_->GetAngle(),
					 body_->GetAngularVelocity()};
	}

	void Car::setState(const State& state) {
		// Set the position and current angle.
		body_->SetTransform(state.position_ - body_->GetPosition(), state.angle_ - body_->GetAngle());

		// Set the velocity of the states.
		body_->SetAngularVelocity(state.anglularVelocity_);
		body_->SetLinearVelocity(body_->GetLinearVelocity());
	}

	bool Car::isInsideViewArea(Position position) const {
		return true;
	}

	void Car::createBody(b2World& world) {
		// Box2d properties.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Zero;//state.position_;
		bodyDef.angle = 0;// state.angle_;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
		
		body_ = world.CreateBody(&bodyDef);

		// Body properties.
		{
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(0.5f *length_, 0.5f * width_); // Expected parameters is half the side.

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = mass_ / (length_ * width_);
			fixtureDef.friction = 0.3f;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			b2Fixture* fixture = body_->CreateFixture(&fixtureDef);
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
