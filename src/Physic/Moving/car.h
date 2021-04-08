#ifndef ZOMBIE_CAR_H
#define ZOMBIE_CAR_H

#include "Physic/Moving/movingobject.h"
#include "input.h"
#include "Physic/state.h"

namespace zombie {

	class Unit;

	// Defines the property of a car. The car has 4 wheels but is simulated as having 
	// one front wheel and one backwheel in order to simlify the math.
	class Car : public MovingObject {
	public:
		enum CarEvent {
			Action,
			Moved
		};

		Car(float mass, float life, float width, float length);
		virtual ~Car() = default;

		Car(const Car& car);
		Car& operator=(const Car& car);

		Unit* getDriver() const;
		void setDriver(Unit* driver);

		void updatePhysics(double time, double timeStept);

		void applySpin(float impulse) {
			body_->ApplyAngularImpulse(impulse, true);
		}

		State getState() const;

		void setState(const State& state);

		float getWidth() const {
			return width_;
		}

		float getLength() const {
			return length_;
		}

		bool isInsideViewArea(Position position) const override;

		Position getPosition() const {
			return body_->GetPosition();
		}

		b2Body* getBody() const override {
			return body_;
		}

		float getDirection() const {
			return body_->GetAngle();
		}

		bool isInfected() const override {
			// No driver, the car is seen as infected and therefore ignored by the zombies.
			return driver_ == nullptr;
		}

		float getViewDistance() const override {
			return 10;
		}

		bool isDead() const override {
			return false;
		}

	protected:
		inline State previousState() const {
			return previousState_;
		}

	private:
		void createBody(b2World& world) override;

		void destroyBody() override {
			if (body_ != nullptr) {
				b2World* world = body_->GetWorld();
				if (world != nullptr) {
					world->DestroyBody(body_);
				}
				body_ = nullptr;
			}
		}

		void applyFriction(bool brake, float frictionForwardFrontWheel, float frictionForwardBackWheel,
			float frictionLateralFrontWheel, float frictionLateralBackWheel);

		b2Vec2 getFrontWheelPosition() const {
			return body_->GetWorldPoint(b2Vec2(length_ * wheelDelta_, 0));
		}

		b2Vec2 getBackWheelPosition() const {
			return body_->GetWorldPoint(b2Vec2(-length_ * wheelDelta_, 0));
		}

		b2Vec2 getDirectionVector() const {
			return body_->GetWorldVector(b2Vec2(std::cos(steeringAngle_), std::sin(steeringAngle_)));
		}

		b2Body* body_{};
		float steeringAngle_{0.f};
		
		float length_;
		float width_;
		float mass_;
		float currentTime_{0.f};
		float wheelDelta_{0.4f};

		State previousState_;
		Unit* driver_{};
	};

}

#endif
