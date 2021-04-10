#ifndef ZOMBIE_WEAPONINTERFACE_H
#define ZOMBIE_WEAPONINTERFACE_H

#include "box2ddef.h"

#include <memory>

namespace zombie {

	class Unit;
	class GameInterface;

	class WeaponInterface;
	using WeaponInterfacePtr = std::shared_ptr<WeaponInterface>;

	// Describes a Weapon and is responsible of shooting.
	class WeaponInterface {
	public:
		virtual ~WeaponInterface() = default;

		virtual void pullTrigger(Unit& unit, float time) = 0;

		virtual void releaseTrigger(Unit& unit, float time) = 0;
		
		virtual void reload(float time) = 0;

		virtual float getRange() const = 0;

		virtual int getClipSize() const = 0;

		virtual int getBulletsInWeapon() const = 0;

		// Must be called by ZombieEngine.
		virtual void initEngine(b2World* world_, GameInterface* gameInterface) {
		}

		virtual WeaponInterfacePtr clone() const = 0;
	};

}

#endif
