#ifndef ZOMBIE_HUMANPLAYER_H
#define ZOMBIE_HUMANPLAYER_H

#include "device.h"
#include "Physic/Moving/unit.h"
#include "player.h"

namespace zombie {

	class HumanPlayer : public Player {
	public:
		HumanPlayer(DevicePtr device, Unit* unit)
			: device_{std::move(device)}
			, unit_{unit} {
		}

		void updateInput(double time, double deltaTime) override {
			unit_->setInput(device_->nextInput());
		}

		Unit* getUnit() const {
			return unit_;
		}

	private:
		DevicePtr device_;
		Unit* unit_;
	};

}

#endif
