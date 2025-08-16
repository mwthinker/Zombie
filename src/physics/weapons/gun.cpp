#include "gun.h"
#include "physics/weapons/weaponinterface.h"
#include "physics/state.h"
#include "physics/moving/unit.h"
#include "box2ddef.h"
#include "physics/gameinterface.h"

namespace zombie {

	namespace {

		void doShotDamage(b2WorldId worldId, GameInterface& gameInterface, float damage, float angle, float range, Unit& shooter) {
			b2Vec2 dir{std::cos(angle), std::sin(angle)};
			b2Vec2 hitPosition = shooter.getPosition() + range * dir;

			auto filter = b2DefaultQueryFilter();

			struct Data {
				Unit& shooter;
				float damage;
				GameInterface& gameInterface;
			};
			Data data{
				.shooter = shooter,
				.damage = damage,
				.gameInterface = gameInterface
			};

			auto callback = [](b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
				if (!b2Shape_IsSensor(shapeId)) {
					return -1.f;
				}
				if (auto ob = b2Shape_GetUserData(shapeId)) {
					auto po = static_cast<PhysicalObject*>(ob);
					Data& data = *static_cast<Data*>(context);

					if (auto target = dynamic_cast<Unit*>(po)) {
						data.gameInterface.shotHit(data.shooter.getPosition(), point, *target);

						if (!target->isDead()) {
							target->updateHealthPoint(-data.damage);
						}
					} else {
						// Calculate the hit position on the unknown object.
						data.gameInterface.shotMissed(data.shooter.getPosition(), point);
					}
					return 0.f;
				}
				return -1.f;
			};

			b2World_CastRay(worldId, shooter.getPosition(), hitPosition, filter, callback, &data);
		}

	}

	Gun::Gun(float damage, float timeBetweenShots, float range, int clipSize) {
		damage_ = damage;
		timeBetweenShots_ = timeBetweenShots;
		range_ = range;
		clipSize_ = clipSize;
		bulletsInWeapon_ = clipSize;
		lastShotTime_ = 0.0f;
	}

	void Gun::pullTrigger(Unit& unit, float time) {
		if (lastShotTime_ + timeBetweenShots_ < time) {
			if (bulletsInWeapon_ > 0) {
				lastShotTime_ = time;
				--bulletsInWeapon_;
				doShotDamage(worldId_, *gameInterface_, damage_, unit.getDirection(), range_, unit);
			}
		}
	}

	void Gun::releaseTrigger(Unit& unit, float time) {
	}

	int Gun::getClipSize() const {
		return clipSize_;
	}

	int Gun::getBulletsInWeapon() const {
		return bulletsInWeapon_;
	}

	// Tries to reload the weapon. If it reloads return true, else false.
	void Gun::reload(float time) {
		if (bulletsInWeapon_ == clipSize_) {
			// No need to reload.
		} else {
			bulletsInWeapon_ = clipSize_;
		}
	}

	float Gun::getRange() const {
		return range_;
	}

	void Gun::initEngine(b2WorldId worldId, GameInterface* gameInterface) {
		gameInterface_ = gameInterface;
		worldId_ = worldId;
	}

}
