#ifndef ZOMBIE_WEAPON_H
#define ZOMBIE_WEAPON_H

#include "weaponinterface.h"
#include "physics/state.h"
#include "physics/gameinterface.h"

#include <sdl/sprite.h>

#include <memory>

namespace zombie {

	class Weapon;
	using WeaponPtr = std::shared_ptr<Weapon>;

	class Weapon {
	public:
		Weapon() = default;

		explicit Weapon(bool laserSight)
			: weaponInterface_{nullptr}
			, laserSight_{laserSight} {
		}

		Weapon(const Weapon& weapon) = delete;
		Weapon& operator=(const Weapon& weapon) = delete;

		Weapon(const WeaponInterfacePtr& weaponInterface)
			: weaponInterface_{weaponInterface} {
		}

		void pullTrigger(Unit& shooter, float time) {
			if (weaponInterface_) {
				weaponInterface_->pullTrigger(shooter, time);
			}
		}

		void releaseTrigger(Unit& shooter, float time) {
			if (weaponInterface_) {
				weaponInterface_->releaseTrigger(shooter, time);
			}
		}

		// Tries to reload the weapon. If it reloads return true, else false.
		void reload(float time) {
			if (weaponInterface_) {
				weaponInterface_->reload(time);
			}
		}

		int getClipSize() const {
			return weaponInterface_->getClipSize();
		}

		int getBulletsInWeapon() const {
			if (weaponInterface_) {
				return weaponInterface_->getBulletsInWeapon();
			}
			return 0;
		}

		float getRange() const {
			if (weaponInterface_) {
				return weaponInterface_->getRange();
			}
			return 0;
		}

		// Should be called by the zombieEngine.
		void init(b2World* world, GameInterface* gameInterface) {
			if (weaponInterface_) {
				weaponInterface_->initEngine(world, gameInterface);
			}
		}

		bool hasLaserSight() const {
			return laserSight_;
		}

	private:
		WeaponInterfacePtr weaponInterface_{};
		bool laserSight_{};
	};

}

#endif
