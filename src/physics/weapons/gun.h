#ifndef ZOMBIE_GUN_H
#define ZOMBIE_GUN_H

#include "weaponinterface.h"
#include "physics/state.h"
#include "box2ddef.h"
#include "weaponproperties.h"

#include <memory>

namespace zombie {

	class Gun : public WeaponInterface {
	public:
		Gun(float damage, float timeBetweenShots, float range, int clipSize);

		void pullTrigger(Unit& unit, float time) override;

		void releaseTrigger(Unit& unit, float time) override;

		int getClipSize() const override;

		int getBulletsInWeapon() const override;

		void reload(float time) override;

		float getRange() const override;

		void initEngine(b2World* world, GameInterface* gameInterface);

		WeaponInterfacePtr clone() const override;

	private:
		GameInterface* gameInterface_{};
		b2World* world_{};

		float damage_;				// The damage made by the weapon.
		float timeBetweenShots_;
		float range_;				// The range of the weapon.

		int clipSize_;				// The number of bullets for a reload.
		int bulletsInWeapon_;		// The current number of bullets in the weapon.
		float lastShotTime_;
	};

}

#endif
