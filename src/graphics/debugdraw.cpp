#include "debugdraw.h"
#include "box2ddef.h"

#include <concepts>

namespace {

	inline sdl::Color toColor(const b2Color& color) {
		return sdl::Color{color.r, color.g, color.b, color.a};
	}

	constexpr float LineWidth = 0.1f;

	void iterate(const b2Vec2* vertices, int32 vertexCount, std::invocable<glm::vec2> auto&& call) {
		for (int i = 0; i < vertexCount; ++i) {
			const auto& vertex = vertices[vertexCount];
			call(glm::vec2{vertex.x, vertex.y});
		}
	}

}

namespace zombie {

	void DebugDraw::SetGraphic(sdl::Graphic* graphic) {
		graphic_ = graphic;
	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		static std::vector<glm::vec2> sdlVertices;

		sdlVertices.clear();

		iterate(vertices, vertexCount, [&](const glm::vec2& v) {
			sdlVertices.push_back(v);
		});

		graphic_->addPixelLine(sdlVertices.begin(), sdlVertices.end(), toColor(color));
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		static std::vector<glm::vec2> sdlVertices;

		sdlVertices.clear();

		iterate(vertices, vertexCount, [&](const glm::vec2& v) {
			sdlVertices.push_back(v);
		});

		graphic_->addPolygon(sdlVertices.begin(), sdlVertices.end(), toColor(color));
		
		DrawPolygon(vertices, vertexCount, color);
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		graphic_->addCircleOutline({center.x, center.y}, radius, LineWidth, toColor(color));
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		graphic_->addCircle({center.x, center.y}, radius, toColor(color));
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		graphic_->addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, toColor(color));
	}

	void DebugDraw::DrawTransform(const b2Transform& xf) {
		if (!graphic_) {
			return;
		}

		const float k_axisScale = 0.4f;
		b2Vec2 p1 = xf.p;
		b2Vec2 p2 = p1 + k_axisScale * xf.q.GetXAxis();
		graphic_->addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, sdl::color::html::Red);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		graphic_->addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, sdl::color::html::Green);
	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
		if (!graphic_) {
			return;
		}

		graphic_->addPixel({p.x, p.y}, toColor(color), size);
	}

}
