#ifndef ZOMBIE_WEAPONITEM_H
#define ZOMBIE_WEAPONITEM_H

#include "Physic/physicalobject.h"
#include "weapon.h"

namespace zombie {

	class WeaponItem : public PhysicalObject {
	public:
		WeaponItem(float x, float y, WeaponPtr weapon)
			: weapon_{std::move(weapon)} {
		}

		void createBody(b2World* world) {
			// Box2d properties.
			b2BodyDef bodyDef;
			bodyDef.position.Set(x_, y_);
			bodyDef.angle = 0.0;
			bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			body_ = world->CreateBody(&bodyDef);

			// Add tensor. Should not be a physical object.
			{
				b2CircleShape circle;
				circle.m_p.Set(0, 0);
				circle.m_radius = radius_;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circle;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				fixtureDef.isSensor = true;
				fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				// Add Body fixture.
				auto fixture = body_->CreateFixture(&fixtureDef);
			}
		}

		const WeaponPtr& getWeapon() const {
			return weapon_;
		}

		Position getPosition() const {
			return body_->GetPosition();
		}

		float getRadius() const {
			return radius_;
		}

		b2Body* getBody() const override {
			return body_;
		}

	private:
		WeaponPtr weapon_;
		float radius_{0.5f};
		float x_{0.f};
		float y_{0.f};


		b2Body* body_{};
	};

}

#endif
