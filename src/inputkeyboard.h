#ifndef ZOMBIE_INPUTKEYBOARD_H
#define ZOMBIE_INPUTKEYBOARD_H

#include "device.h"

#include <SDL.h>

namespace zombie {

	struct KeyboardKeys {
		SDL_Keycode up;
		SDL_Keycode down;
		SDL_Keycode right;
		SDL_Keycode left;
		SDL_Keycode shoot;
		SDL_Keycode reload;
		SDL_Keycode run;
		SDL_Keycode action;
	};

	class InputKeyboard : public Device {
	public:
		InputKeyboard(const KeyboardKeys& keyCodes)
			: keyCodes_{keyCodes} {
		}

		void eventUpdate(const SDL_Event& windowEvent) override {
			auto key = windowEvent.key.keysym.sym;

			switch (windowEvent.type) {
			case SDL_KEYDOWN:
				if (key == keyCodes_.up) {
					input_.forward = true;
				} else if (key == keyCodes_.down) {
					input_.backward = true;
				} else if (key == keyCodes_.left) {
					input_.turnLeft = true;
				} else if (key == keyCodes_.right) {
					input_.turnRight = true;
				} else if (key == keyCodes_.shoot) {
					input_.shoot = true;
				} else if (key == keyCodes_.reload) {
					input_.reload = true;
				} else if (key == keyCodes_.run) {
					input_.run = true;
				} else if (key == keyCodes_.action) {
					input_.action = true;
				}
				break;
			case SDL_KEYUP:
				if (key == keyCodes_.up) {
					input_.forward = false;
				} else if (key == keyCodes_.down) {
					input_.backward = false;
				} else if (key == keyCodes_.left) {
					input_.turnLeft = false;
				} else if (key == keyCodes_.right) {
					input_.turnRight = false;
				} else if (key == keyCodes_.run) {
					input_.run = false;
				}
				break;
			}
		}

		Input nextInput() override {
			Input input = input_;
			input_.shoot  = false;
			input_.reload = false;
			input_.action = false;
			return input;
		}

	private:
		Input input_{};
		KeyboardKeys keyCodes_{};
	};

}

#endif
