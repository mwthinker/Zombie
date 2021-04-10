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

		Weapon(const Weapon&& weapon) noexcept = delete;
		Weapon& operator=(const Weapon&& weapon) noexcept = delete;

		Weapon(const Weapon& weapon) {
			weaponInterface_ = weapon.weaponInterface_->clone();
		}

		Weapon& operator=(const Weapon& weapon) {
			weaponInterface_ = weapon.weaponInterface_->clone();
			return *this;
		}

		Weapon(const WeaponInterfacePtr& weaponInterface)
			: weaponInterface_{weaponInterface} {
		}

		void pullTrigger(Unit& shooter, float time) {
			weaponInterface_->pullTrigger(shooter, time);
		}

		void releaseTrigger(Unit& shooter, float time) {
			weaponInterface_->releaseTrigger(shooter, time);
		}

		// Tries to reload the weapon. If it reloads return true, else false.
		void reload(float time) {
			weaponInterface_->reload(time);
		}

		int getClipSize() const {
			return weaponInterface_->getClipSize();
		}

		int getBulletsInWeapon() const {
			return weaponInterface_->getBulletsInWeapon();
		}

		// Return the weapon symbol.
		virtual const sdl::Sprite& getSymbol() const = 0;

		// Draw the weapon. The function will be called when the physical object is drawn.
		//virtual void draw(float timeStep, float x, float y, const GameShader& shader) = 0;

		float getRange() const {
			return weaponInterface_->getRange();
		}

		virtual WeaponPtr clone() const = 0;

		// Should be called by the zombieEngine.
		void init(b2World* world, GameInterface* gameInterface) {
			weaponInterface_->initEngine(world, gameInterface);
		}

		bool hasLaserSight() const {
			return laserSight_;
		}

		virtual void updateLaserSight(b2World* world, float time, Position position, float angle) = 0;

	private:
		WeaponInterfacePtr weaponInterface_{};
		bool laserSight_{};
	};

}

#endif
