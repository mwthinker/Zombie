#ifndef ZOMBIE_WEAPONITEM_H
#define ZOMBIE_WEAPONITEM_H

#include "physics/physicalobject.h"
#include "weapon.h"

namespace zombie {

	class WeaponItem : public PhysicalObject {
	public:
		WeaponItem(float x, float y, WeaponPtr weapon)
			: weapon_{std::move(weapon)} {
		}

		const WeaponPtr& getWeapon() const {
			return weapon_;
		}

		Position getPosition() const {
			return b2Body_GetPosition(bodyId_);
		}

		float getRadius() const {
			return radius_;
		}

		b2BodyId getBody() const override {
			return bodyId_;
		}

	private:
		void createBody(b2WorldId worldId) override {
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = b2Vec2{x_, y_};
			bodyDef.rotation = b2MakeRot(0);
			bodyDef.userData = this;

			bodyId_ = b2CreateBody(worldId, &bodyDef);
			// Add tensor. Should not be a physical object.
			{
				b2Circle circle{
					.center = Zero,
					.radius = radius_
				};

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = 0.0f;
				//shapeDef.friction = 0.0f;
				shapeDef.isSensor = true;
				shapeDef.userData = this;

				auto circleShapeId = b2CreateCircleShape(bodyId_, &shapeDef, &circle);
			}
		}

		void destroyBody() override {
			if (b2Body_IsValid(bodyId_)) {
				b2DestroyBody(bodyId_);
			}
		}

		WeaponPtr weapon_;
		float radius_ = 0.5f;
		float x_ = 0.f;
		float y_ = 0.f;
		b2BodyId bodyId_ = b2_nullBodyId;
	};

}

#endif
