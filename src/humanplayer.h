#ifndef ZOMBIE_HUMANPLAYER_H
#define ZOMBIE_HUMANPLAYER_H

#include "device.h"
#include "physics/moving/unit.h"
#include "player.h"
#include "physics/moving/unit.h"

#include <glm/gtx/rotate_vector.hpp>

namespace zombie {

	class HumanPlayer : public Player {
	public:
		HumanPlayer(DevicePtr device, std::unique_ptr<Unit> unit)
			: device_{std::move(device)}
			, unit_{std::move(unit)} {
		}

		void updateInput(double time, double deltaTime) override {
			unit_->setInput(device_->nextInput());
			unit_->updatePhysics(time, deltaTime);
		}

		void draw(sdl::Graphic& graphic) override {
			auto pos = unit_->getPosition();
			graphic.addCircle({pos.x, pos.y}, unit_->getRadius(), sdl::color::html::DeepSkyBlue);
			graphic.addCircleOutline({pos.x, pos.y}, unit_->getViewDistance(), 0.1f, sdl::color::html::Firebrick);
			graphic.addLine({pos.x, pos.y}, glm::vec2{pos.x, pos.y} + glm::rotate(glm::vec2{1.f, 0.f}, unit_->getDirection()), 0.1f, sdl::color::html::Firebrick);
		}

		Unit* getUnit() const {
			return unit_.get();
		}

		PhysicalObject* getPhysicalObject() override {
			return unit_.get();
		}

	private:
		DevicePtr device_;
		std::unique_ptr<Unit> unit_;
	};

	using HumanPlayerPtr = std::unique_ptr<HumanPlayer>;

}

#endif
