#ifndef ZOMBIE_BOX2DDEF_H
#define ZOMBIE_BOX2DDEF_H

#include <sdl/color.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <fmt/format.h>
#include <fmt/color.h>

#include <string>

#include <box2d/box2d.h>

#include <vector>

namespace zombie {

	const b2Vec2 Origo{0, 0};
	const b2Vec2 Zero{0, 0};

	using Velocity = b2Vec2;
	using Position = b2Vec2;
	using Force = b2Vec2;

	constexpr auto Pi = glm::pi<float>();

	using Color = sdl::Color;

	constexpr Color White = sdl::color::html::White;
	constexpr Color Black = sdl::color::html::Black;
	constexpr Color Red = sdl::color::html::Red;

}

template <>
struct fmt::formatter<sdl::Color> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sdl::Color color, FormatContext& ctx) {
		return formatter<std::string>::format(color.toHexString(), ctx);
	}
};

template <>
struct fmt::formatter<b2Vec2> : nested_formatter<decltype(b2Vec2::x)> {
	auto format(b2Vec2 p, format_context& ctx) const {
		return write_padded(ctx, [=](auto out) {
			return fmt::format_to(out, "({}, {})", nested(p.x), nested(p.y));
		});
	}
};

template <>
struct fmt::formatter<glm::vec2> : nested_formatter<decltype(glm::vec2::x)> {
	auto format(glm::vec2 p, format_context& ctx) const {
		return write_padded(ctx, [=](auto out) {
			return fmt::format_to(out, "({}, {})", nested(p.x), nested(p.y));
		});
	}
};

template <>
struct fmt::formatter<glm::vec3> : nested_formatter<decltype(glm::vec3::x)> {
	auto format(glm::vec3 p, format_context& ctx) const {
		return write_padded(ctx, [=](auto out) {
			return fmt::format_to(out, "({}, {})", nested(p.x), nested(p.y));
		});
	}
};

template <>
struct fmt::formatter<glm::vec4> : nested_formatter<decltype(glm::vec4::x)> {
	auto format(glm::vec4 p, format_context& ctx) const {
		return write_padded(ctx, [=](auto out) {
			return fmt::format_to(out, "({}, {})", nested(p.x), nested(p.y));
		});
	}
};

#endif
