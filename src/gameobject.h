#ifndef ZOMBIE_GAMEOBJECT_H
#define ZOMBIE_GAMEOBJECT_H

#include "device.h"

#include <sdl/graphic.h>

#include <fmt/printf.h>

#include <memory>
#include <map>
#include <vector>

namespace zombie {

	class GameObject {
	public:
		virtual ~GameObject() = default;

		virtual void update(double deltaTime) = 0;
		virtual void draw(sdl::Graphic& graphic) = 0;

	private:
		
	};

	using GameObjectPtr = std::unique_ptr<GameObject>;

	class Unit;
	class HumanGameObject : public GameObject {
	public:
		HumanGameObject(Unit& unit) 
			: unit_{unit} {
		}

		void update(double deltaTime) override {

		}

		void draw(sdl::Graphic& graphic) override {

		}
	private:
		Unit& unit_;
	};

}
#endif
