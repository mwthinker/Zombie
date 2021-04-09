#include "auxiliary.h"

#include <random>

namespace zombie {

	namespace {

		std::random_device rd;
		std::default_random_engine generator{rd()};

	}

	float random(float min, float max) {
		return std::uniform_real_distribution{min, max}(generator);
	}

	float random() {
		return random(0.f, 1.f);
	}

	int randomInt(int lower, int higher) {
		return std::uniform_int_distribution<int>{lower, higher}(generator);
	}

}
