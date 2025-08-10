#ifndef ZOMBIE_TEXTUREVIEW_H
#define ZOMBIE_TEXTUREVIEW_H

#include "input.h"
#include "box2ddef.h"

#include <SDL3/SDL_gpu.h>

namespace zombie {

	struct TextureView {
		glm::vec2 position;
		glm::vec2 tex;
	};

}

#endif
