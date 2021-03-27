#ifndef ZOMBIE_TREE_H
#define ZOMBIE_TREE_H

#include "box2ddef.h"
#include "auxiliary.h"
#include "Physic/physicalobject.h"

#include <memory>

namespace zombie {

	class Tree : public PhysicalObject {
	public:
		Tree(const Position& position)
			: position_{position} {

			radius_ = random(0.5f, 2);
			angle_ = random(0, 2*PI);
		}

		void createBody(b2World* world) {
			// Box2d properties.
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position = position_;
			bodyDef.angle = angle_;
			bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			body_ = world->CreateBody(&bodyDef);

			b2CircleShape circle;
			circle.m_p.Set(0, 0);
			circle.m_radius = radius_;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circle;
			fixtureDef.friction = 0.0f;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			
			// Add Body fixture.
			b2Fixture* fixture = body_->CreateFixture(&fixtureDef);
		}

		float getRadius() const {
			return radius_;
		}

		Position getPosition() const {
			return position_;
		}

		float getAngle() const {
			return angle_;
		}

		b2Body* getBody() const override {
			return body_;
		}

		void destroyBody() override {
			if (body_ != nullptr) {
				b2World* world = body_->GetWorld();
				if (world != nullptr) {
					world->DestroyBody(body_);
				}
				body_ = nullptr;
			}
		}

	private:
		Position position_;
		float radius_;
		float angle_;
		b2Body* body_{};
	};

}

#endif
