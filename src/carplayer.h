#ifndef ZOMBIE_CARPLAYER_H
#define ZOMBIE_CARPLAYER_H

#include "box2ddef.h"
#include "input.h"
#include "player.h"
#include "physics/moving/car.h"
#include "device.h"

namespace zombie {

	class CarPlayer : public Player {
	public:
		explicit CarPlayer(DevicePtr device, std::unique_ptr<Car> car);

		void updateInput(double time, double deltaTime) override;
		void draw(sdl::Graphic& graphic) override;

		PhysicalObject* getPhysicalObject() override;

	private:
		DevicePtr device_;
		std::unique_ptr<Car> car_;
	};

	using CarPlayerPtr = std::unique_ptr<CarPlayer>;

}

#endif
