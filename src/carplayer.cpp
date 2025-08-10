#include "auxiliary.h"
#include "carplayer.h"

#include <list>
#include <cassert>

#include <glm/gtx/rotate_vector.hpp>

namespace zombie {

	CarPlayer::CarPlayer(DevicePtr device, std::unique_ptr<Car> car)
		: device_{device}
		, car_{std::move(car)} {
	}

	void CarPlayer::updateInput(double time, double deltaTime) {
		car_->setInput(device_->nextInput());
		car_->updatePhysics(time, deltaTime);
	}

	void CarPlayer::draw(Graphic& graphic) {
		auto pos = car_->getPosition();
		//pos = {3.f, 3.f};
		graphic.pushMatrix();
		graphic.translate({pos.x, pos.y});
		graphic.rotate(car_->getDirection());
		
		graphic.rotate(Pi/2);
		graphic.addRectangle({-car_->getLength() * 0.5f, -car_->getWidth() * 0.5f}, {car_->getLength(), car_->getWidth()}, sdl::color::html::HotPink);

		graphic.popMatrix();
		
		//graphic.addCircle({pos.x, pos.y}, unit_->getRadius(), sdl::color::html::Firebrick);
		//graphic.addCircleOutline({pos.x, pos.y}, unit_->getViewDistance(), 0.04f, sdl::color::html::Firebrick);
	}

	PhysicalObject* CarPlayer::getPhysicalObject() {
		return car_.get();
	}

}
