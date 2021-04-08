#ifndef ZOMBIE_HUMANPLAYER_H
#define ZOMBIE_HUMANPLAYER_H

#include "device.h"
#include "Physic/Moving/unit.h"
#include "player.h"
#include "Physic/Moving/unit.h"

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
		}

		Unit* getUnit() const {
			return unit_.get();
		}

	private:
		DevicePtr device_;
		std::unique_ptr<Unit> unit_;
	};

	using HumanPlayerPtr = std::unique_ptr<HumanPlayer>;

}

#endif
