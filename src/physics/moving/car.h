#ifndef ZOMBIE_CAR_H
#define ZOMBIE_CAR_H

#include "physics/moving/movingobject.h"
#include "input.h"
#include "physics/state.h"

#include <mw/signal.h>

namespace zombie {

	enum class CarEvent {
		Action,
		Moved
	};

	struct CarProperties {
		float mass = 10.f;
		float life = 100.f;
		float width = 1.f;
		float length = 2.f;
	};

	// Defines the property of a car. The car has 4 wheels but is simulated as having 
	// one front wheel and one backwheel in order to simlify the math.
	class Car : public MovingObject {
	public:
		mw::PublicSignal<Car, CarEvent> carEventHandler;

		Car(const CarProperties& properties);

		Car(const Car& car) = delete;
		Car& operator=(const Car& car) = delete;

		void updatePhysics(double time, double timeStept);

		void applySpin(float impulse) {
			b2Body_ApplyAngularImpulse(bodyId_, impulse, true);
		}

		State getState() const;

		void setState(const State& state);

		float getWidth() const {
			return properties_.width;
		}

		float getLength() const {
			return properties_.length;
		}

		bool isInsideViewArea(Position position) const override;

		Position getPosition() const {
			return b2Body_GetPosition(bodyId_);
		}

		b2BodyId getBody() const override {
			return bodyId_;
		}

		float getDirection() const {
			//return body_->GetAngle();
			return 0.f;
		}

		bool isInfected() const override {
			return false;
		}

		float getViewDistance() const override {
			return 10;
		}

		bool isDead() const override {
			return false;
		}

		inline void setInput(Input input) {
			input_ = input;
		}

		inline Input getInput() const {
			return input_;
		}

	private:
		void createBody(b2WorldId world) override;

		void destroyBody() override {
			if (b2Body_IsValid(bodyId_)) {
				b2DestroyBody(bodyId_);
			}
		}

		void applyFriction(bool brake, float frictionForwardFrontWheel, float frictionForwardBackWheel,
			float frictionLateralFrontWheel, float frictionLateralBackWheel);

		b2Vec2 getFrontWheelPosition() const {
			return b2Body_GetWorldPoint(bodyId_, b2Vec2{properties_.length * wheelDelta_, 0});
		}

		b2Vec2 getBackWheelPosition() const {
			return b2Body_GetWorldPoint(bodyId_, b2Vec2{-properties_.length * wheelDelta_, 0});
		}

		b2Vec2 getDirectionVector() const {
			//return body_->GetWorldVector(b2Vec2(std::cos(steeringAngle_), std::sin(steeringAngle_)));
			return b2Body_GetWorldVector(bodyId_, b2Vec2{std::cos(steeringAngle_), std::sin(steeringAngle_)});
		}

		b2BodyId bodyId_{};
		float steeringAngle_{0.f};
		
		CarProperties properties_{};

		float currentTime_{0.f};
		float wheelDelta_{0.4f};
		Input input_{};
	};

}

#endif
