#include "gun.h"
#include "physics/weapons/weaponinterface.h"
#include "physics/state.h"
#include "physics/moving/unit.h"
#include "box2ddef.h"
#include "physics/closestraycastcallback.h"

namespace zombie {

	namespace {

		void doShotDamage(b2World& world, GameInterface& gameInterface, float damage, float angle, float range, Unit& shooter) {
			b2Vec2 dir{std::cos(angle), std::sin(angle)};
			b2Vec2 hitPosition = shooter.getPosition() + range * dir;

			// Return the closest physcal object.
			ClosestRayCastCallback callback([](b2Fixture* fixture) {
				return !fixture->IsSensor();
			});

			world.RayCast(&callback, shooter.getPosition(), hitPosition);
			auto fixture = callback.getFixture();

			hitPosition = shooter.getPosition() + range * callback.getFraction() * dir;

			// Did bullet hit something?
			if (fixture != nullptr) {
				if (auto target = dynamic_cast<Unit*>(fixture->GetUserData().physicalObject)) {
					gameInterface.shotHit(shooter.getPosition(), hitPosition, *target);
					
					if (!target->isDead()) {
						target->updateHealthPoint(-damage);
					}
				} else {
					// Calculate the hit position on the unknown object.
					gameInterface.shotMissed(shooter.getPosition(), hitPosition);
				}
			} else {
				gameInterface.shotMissed(shooter.getPosition(), hitPosition);
			}
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
				doShotDamage(*world_, *gameInterface_, damage_, unit.getDirection(), range_, unit);
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

	void Gun::initEngine(b2World* world, GameInterface* gameInterface) {
		gameInterface_ = gameInterface;
		world_ = world;
	}

}
