#ifndef ZOMBIE_DEVICE_H
#define ZOMBIE_DEVICE_H

#include "input.h"

#include <SDL.h>

#include <memory>

namespace zombie {

	class Device;
	using DevicePtr = std::shared_ptr<Device>;

	class Device {
	public:
		virtual ~Device() = default;

		virtual void eventUpdate(const SDL_Event& windowEvent) = 0;

		virtual Input nextInput() = 0;
	};

}

#endif
