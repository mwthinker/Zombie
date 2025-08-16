#include "missile.h"
#include "physics/moving/unit.h"
#include "physics/gameinterface.h"
#include "auxiliary.h"

namespace zombie {

	Missile::Missile(GameInterface* gameInterface, float width, float length, float mass)
		: gameInterface_{gameInterface}
		, length_{length}
		, width_{width}
		, mass_{mass} {
	}

	void Missile::create(Position position, float angle, float speed, float explodeTime, float damage, float explosionRadius, float force) {
		speed_ = speed;
		damage_ = damage;
		explosionRadius_ = explosionRadius;
		explode_ = false;
		explodeTime_ = explodeTime;
		time_ = 0;
		force_ = force;

		b2Body_SetAwake(bodyId_, true);
		// Set the position and current angle.
		b2Body_SetTransform(bodyId_, position, b2MakeRot(angle));

		// Set the velocity of the states.
		b2Body_SetLinearVelocity(bodyId_, speed_ * directionVector(angle));
		b2Body_SetAngularVelocity(bodyId_, 0);
	}

	void Missile::createBody(b2WorldId world) {
		b2BodyDef bodyDef{
			.type = b2_dynamicBody,
			.position = Zero,
		};
		b2Body_SetUserData(bodyId_, this);
		//bodyDef.userData.physicalObject = this.

		bodyId_ = b2CreateBody(world, &bodyDef);

		// Body properties.
		{
			b2Polygon dynamicBox = b2MakeBox(0.5f * length_, 0.5f * width_); // Expected parameters is half the side.

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = mass_ / (length_ * width_);
			//shapeDef.friction = 0.3f;
			shapeDef.userData = this;

			b2CreatePolygonShape(bodyId_, &shapeDef, &dynamicBox);
		}

		previousState_ = getState();
	}

	void Missile::collision(float impulse) {
		explode_ = true;
	}

	void Missile::updatePhysics(double time, double timeStep) {
		previousState_ = getState();
		time_ += (float) timeStep;

		if (explode_ || explodeTime_ < time_) {
			explode();
		}
	}

	b2BodyId Missile::getBody() const {
		return bodyId_;
	}

	float Missile::getWidth() const {
		return width_;
	}

	float Missile::getLength() const {
		return length_;
	}

	Position Missile::getPosition() const {
		return b2Body_GetPosition(bodyId_);
	}

	float Missile::getDirection() const {
		return 0.f;// body_->GetAngle();
	}

	State Missile::getState() const {
		return State{
			getPosition(),
			b2Body_GetLinearVelocity(bodyId_),
			b2Rot_GetAngle(b2Body_GetRotation(bodyId_)),
			b2Body_GetAngularVelocity(bodyId_)
		};
	}

	State Missile::previousState() const {
		return previousState_;
	}

	void Missile::explode() {
		float angle = getDirection();
		Position explosionPosition = getPosition() + 0.5f * length_ * Position(std::cos(angle), std::sin(angle));
		b2AABB aabb{
			.lowerBound = explosionPosition - explosionRadius_ * Position(0.5f, 0.5f),
			.upperBound = explosionPosition + explosionRadius_ * Position(0.5f, 0.5f)
		};

		auto worldId = b2Body_GetWorld(bodyId_);
		auto filter = b2DefaultQueryFilter();

		// Perform the query
		struct Data {
			float damage;
			Position explosionPosition;
			float explosionRadius;
			float force;
		};

		Data data{
			.damage = damage_,
			.explosionPosition = explosionPosition,
			.explosionRadius = explosionRadius_,
			.force = force_
		};

		auto overlapCallback = [](b2ShapeId shapeId, void* context) -> bool {
			auto shape = b2Shape_GetUserData(shapeId);
			Data& data = *static_cast<Data*>(context);

			if (void* userData = b2Shape_GetUserData(shapeId)) {
				auto po = static_cast<PhysicalObject*>(userData);

				if (auto unit = dynamic_cast<Unit*>(po)) {
					unit->updateHealthPoint(-data.damage);
					Position dir = b2Normalize(unit->getPosition() - data.explosionPosition);

					// Apply some out going force to the object.
					b2Body_ApplyForceToCenter(unit->getBody(), data.force * dir, true);
				}
			}
			return true; // Continue processing
		};
		b2World_OverlapAABB(worldId, aabb, b2DefaultQueryFilter(), overlapCallback, &data);
		explode_ = false;

		gameInterface_->explosion(explosionPosition, explosionRadius_);
		b2Body_SetAwake(bodyId_, false);
	}

}
