#ifndef ZOMBIE_BOX2DDEF_H
#define ZOMBIE_BOX2DDEF_H

#include <sdl/color.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <fmt/format.h>

#include <iostream>

#include <Box2D/Box2D.h>

#include <vector>

namespace zombie {

	const b2Vec2 Origo{};
	const b2Vec2 Zero{};

	// Math vectors.
	using Velocity = b2Vec2;
	using Position = b2Vec2;
	using Force = b2Vec2;
	using Vec3 = b2Vec2;
	using Point = b2Vec2;
	using Points = std::vector<Position>;

	using Mat4 = glm::mat4;
	using Mat3 = glm::mat3;
	using Mat2 = glm::mat2;
	using Vec4 = glm::vec4;

	constexpr auto Pi = glm::pi<float>();

	using Color = sdl::Color;

	static constexpr Color White = sdl::color::html::White;
	static constexpr Color Black = sdl::color::html::Black;
	static constexpr Color Red = sdl::color::html::Red;

}

#endif
