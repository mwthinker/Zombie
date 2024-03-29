#ifndef ZOMBIE_AUXILIARY_H
#define ZOMBIE_AUXILIARY_H

#include "box2ddef.h"

#include <cmath>
#include <concepts>

namespace zombie {

	template <typename T> requires std::floating_point<T>
	T calculateDifferenceBetweenAngles(T firstAngle, T secondAngle) {
		T difference = firstAngle - secondAngle;
		while (difference < -Pi) difference += 2 * Pi;
		while (difference > Pi) difference -= 2 * Pi;
		return difference;
	}

	inline float calculateAnglePointToPoint(const b2Vec2& p1, const b2Vec2& p2) {
		return std::atan2(p2.y - p1.y, p2.x - p1.x);
	}

	template <typename T> requires std::floating_point<T>
	T calculateAnglePointToPoint(T x, T y, T pX, T pY) {
		return std::atan2(pY-y, pX-x);
	}
	
	template <typename T> requires std::is_signed_v<T>
	constexpr T sign(T value) {
		return value < 0 ? -1 : 1;
	}

	inline Position directionVector(float angle) {
		return {std::cos(angle), std::sin(angle)};
	}

	// Returns a random number. Uniformly distributed on the interval [0, 1).
	float random();

	// Returns a random number. Uniformly distributed on the interval [a, b).
	float random(float a, float b);

	int randomInt(int a, int b);

}

#endif
