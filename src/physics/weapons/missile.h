#ifndef ZOMBIE_MISSILE_H
#define ZOMBIE_MISSILE_H

#include "physics/physicalobject.h"
#include "physics/state.h"
#include "physics/gameinterface.h"
#include "box2ddef.h"

namespace zombie {

	class GameInterface;

	class Missile : public PhysicalObject {
	public:
		Missile() = default;

		Missile(GameInterface* gameInterface, float width, float length, float mass);

		void create(Position position, float angle, float speed, float explodeTime, float damage, float explosionRadius, float force);

		void updatePhysics(double time, double timeStep);

		void collision(float impulse) override final;
		
		b2BodyId getBody() const override final;

		float getWidth() const;

		float getLength() const;

		Position getPosition() const;

		float getDirection() const;

		State getState() const;

		State previousState() const;

	private:
		void createBody(b2WorldId world) override;

		void destroyBody() override {
			if (b2Body_IsValid(bodyId_)) {
				b2DestroyBody(bodyId_);
			}
		}

		void explode();

		GameInterface* gameInterface_{};
		bool explode_ = false;
		
		float damage_{};
		float explosionRadius_{};
		float force_{};
		float time_{};

		b2BodyId bodyId_{};
		float steeringAngle_{};

		float length_{};
		float width_{};
		float mass_{};
		float explodeTime_{};
		float speed_{};

		State previousState_{};
	};

}

#endif
