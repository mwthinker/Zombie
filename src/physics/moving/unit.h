#ifndef ZOMBIE_UNIT_H
#define ZOMBIE_UNIT_H

#include "physics/moving/movingobject.h"
#include "input.h"
#include "physics/weapons/weapon.h"
#include "physics/state.h"
#include "physics/moving/unitproperties.h"

#include <mw/signal.h>

namespace zombie {

	enum class UnitEvent {
		Die,
		Injured,
		Walk,
		Standstill,
		Run,
		Action
	};

	class Unit : public MovingObject {
	public:
		mw::PublicSignal<Unit, UnitEvent> unitEventHandler;
		
		Unit(const UnitProperties& unitProperties, WeaponPtr weapon);

		Unit(const Unit&) = delete;
		Unit& operator=(const Unit&) = delete;

		void updatePhysics(double time, double timeStep);

		State getState() const;

		void setState(const State& state);

		// Return the view distance.
		float getViewDistance() const;

		// Return the distance of where all things are being visible, 
		// no matter of orientation.
		float smallViewDistance() const;

		// Return the current view direction, i.e. the unit's orientation.
		float viewAngle() const;

		// Return true if the point (x, y) is inside the unit.	
		bool isInside(Position position) const;

		bool isInsideViewArea(Position position) const;

		// Return true if the point (x, y) is inside the units small view distance,
		// where all things are being visible.
		bool isInsideSmalViewDistance(Position position) const;

		// Return the angle for the current 
		float getDirection() const;

		float healthPoints() const;
		void updateHealthPoint(float deltaLife);

		bool isDead() const override;

		bool isInfected() const override {
			return properties_.infected;
		}

		WeaponPtr getWeapon() const {
			return weapon_;
		}

		void setWeapon(const WeaponPtr& weapon) {
			weapon_ = weapon;
		}

		Position getPosition() const override;

		float getRadius() const;

		inline void setInput(Input input) {
			input_ = input;
		}

		inline Input getInput() const {
			return input_;
		}

		b2Body* getBody() const override;

		Position getGrip() const {
			return properties_.grip;
		}

	private:
		void createBody(b2World& world) override;

		void destroyBody() override {
			if (body_ != nullptr) {
				auto world = body_->GetWorld();
				if (world != nullptr) {
					world->DestroyBody(body_);
				}
				body_ = nullptr;
			}
		}

		// Properties
		UnitProperties properties_{};

		float viewDistance_{};
		float viewAngle_{};
		float smallViewDistance_{};
		
		//float walkingSpeed_;
		//float runningSpeed_;
		//float radius_;
		//float mass_;

		// Health
		//float healthPoints_;
		bool isDead_ = false;
		bool died_ = false;

		WeaponPtr weapon_;
		//Position grip_;
		//bool isInfected_;

		float timeLeftToRun_{};

		b2Body* body_{};
		Input input_{};
		Input lastInput_{};
	};

}

#endif
