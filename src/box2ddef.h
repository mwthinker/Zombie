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

#include <Box2D/Box2D.h>

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

template<>
struct fmt::formatter<b2Vec2> {
	template<typename ParseContext>
	auto parse(ParseContext& ctx) {
		return ctx.begin();
	}
	
	template <typename FormatContext>
	auto format(const b2Vec2& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
	}
};

template<>
struct fmt::formatter<glm::vec2> {
	template<typename ParseContext>
	auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec2& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
	}

};

template<>
struct fmt::formatter<glm::vec3> {
	template<typename ParseContext>
	auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec3& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({}, {}, {})", p.x, p.y, p.z);
	}
};

template<>
struct fmt::formatter<glm::vec4> {
	template<typename ParseContext>
	auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec4& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({}, {}, {}, {})", p.x, p.y, p.x, p.w);
	}
};

#endif
