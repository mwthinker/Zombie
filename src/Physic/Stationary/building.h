#ifndef ZOMBIE_BUILDING_H
#define ZOMBIE_BUILDING_H

#include "Physic/physicalobject.h"

#include <limits>
#include <array>
#include <cmath>

namespace zombie {

	class Building : public PhysicalObject {
	public:
		Building(Position p1, Position p2, Position p3, Position p4)
			: corners_{p1, p2, p3, p4} {
		}

		const std::array<Position, 4>& getCorners() const {
			return corners_;
		}

		b2Body* getBody() const override {
			return body_;
		}

	private:
		void createBody(b2World* world) override {
			Position center = 0.5f * (corners_[0] + corners_[2]);
			float w = (corners_[0] - corners_[1]).Length();
			float h = (corners_[2] - corners_[1]).Length();
			float angle = std::atan2(corners_[1].y - corners_[0].y, corners_[1].x - corners_[0].x);

			// Create body.
			{
				b2BodyDef bodyDef;
				bodyDef.fixedRotation = true;
				bodyDef.position = center;
				bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				body_ = world->CreateBody(&bodyDef);
			}

			// Create fixture to body.
			{
				b2PolygonShape box;
				box.SetAsBox(w*0.5f, h*0.5f, Zero, angle); // Expected parameters is half the side.

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &box;
				fixtureDef.friction = 0.3f;
				fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				auto fixture = body_->CreateFixture(&fixtureDef);
			}
		}

		void destroyBody() override {
			if (body_ != nullptr) {
				if (auto world = body_->GetWorld(); world != nullptr) {
					world->DestroyBody(body_);
				}
				body_ = nullptr;
			}
		}

		std::array<Position, 4> corners_;
		float radius_{};
		float length_{};
		float width_{};

		b2Body* body_{};
	};

}

#endif
