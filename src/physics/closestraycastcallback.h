#ifndef ZOMBIE_CLOSESTRAYCASTCALLBACK_H
#define ZOMBIE_CLOSESTRAYCASTCALLBACK_H

#include "box2ddef.h"

#include <type_traits>

namespace zombie {

	template <typename ConditionFunc> requires std::is_invocable_r_v<bool, ConditionFunc, b2Fixture*>
	class ClosestRayCastCallback : public b2RayCastCallback {
	public:
		// ConditionFunc in the form of bool(b2Fixture*)
		explicit ClosestRayCastCallback(ConditionFunc&& conditionFunc)
			: conditionFunc_{std::move(conditionFunc)} {
		}

		b2Fixture* getFixture() const {
			return fixture_;
		}

		// Ray-cast callback.
		float ReportFixture(b2Fixture* fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction) override {
			// Is a physical fixture?
			if (conditionFunc_(fixture)) {
				fixture_ = fixture;
				closestFraction_ = fraction;
			}

			// Fraction to clip the ray for closest hit.
			return fraction;
		}

		float getFraction() const {
			return closestFraction_;
		}

		void reset() {
			closestFraction_ = 1.f;
			fixture_ = nullptr;
		}

	private:
		float closestFraction_ = 1.f;
		b2Fixture* fixture_{};
		ConditionFunc conditionFunc_;
	};

}

#endif
