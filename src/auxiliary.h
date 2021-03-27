#ifndef ZOMBIE_AUXILIARY_H
#define ZOMBIE_AUXILIARY_H

#include "box2ddef.h"

#include <cmath>

namespace zombie {

	template <typename T>
	T calculateDifferenceBetweenAngles(T firstAngle, T secondAngle) {
		static_assert(std::is_signed<T>());
		T difference = firstAngle - secondAngle;
		while (difference < -Pi) difference += 2 * Pi;
		while (difference > Pi) difference -= 2 * Pi;
		return difference;
	}

	inline float calculateAnglePointToPoint(const b2Vec2& p1, const b2Vec2& p2) {
		return std::atan2(p2.y - p1.y, p2.x - p1.x);
	}

	template <typename T>
	T calculateAnglePointToPoint(T x, T y, T pX, T pY) {
		return std::atan2(pY-y, pX-x);
	}

	template <typename T>
	constexpr T sign(const T& value) {
		static_assert(std::is_signed<T>());
		return value < 0 ? -1 : 1;
	}

	static Position directionVector(float angle) {
		return Position(std::cos(angle), std::sin(angle));
	}

	// Returns a random number. The output is uniformly distributed on the interval [a, b).
	float random();

	float random(float min, float max);

	int randomInt(int lower, int higher);

}

#endif
