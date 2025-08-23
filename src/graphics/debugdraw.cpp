#include "debugdraw.h"
#include "box2ddef.h"

#include <sdl/color.h>
#include "graphic.h"

#include <concepts>

namespace zombie {

	namespace {

		struct Context {
			Graphic& graphic;
		};

		inline sdl::Color toColor(b2HexColor color) {
			return sdl::Color{static_cast<ImU32>(static_cast<ImU32>(color | 0xFF000000))};
		}

		constexpr float LineWidth = 0.1f;

		void iterate(const b2Vec2* vertices, int vertexCount, std::invocable<glm::vec2> auto&& call) {
			for (int i = 0; i < vertexCount; ++i) {
				const auto& vertex = vertices[vertexCount];
				call(glm::vec2{vertex.x, vertex.y});
			}
		}

		/// Draw a closed polygon provided in CCW order.
		void drawPolygonFcn(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			static std::vector<glm::vec2> sdlVertices;
			sdlVertices.clear();
			iterate(vertices, vertexCount, [&](const glm::vec2& v) {
				sdlVertices.push_back(v);
			});
			graphic.addPixelLine(sdlVertices.begin(), sdlVertices.end(), toColor(color));
		}

		/// Draw a solid closed polygon provided in CCW order.
		void drawSolidPolygonFcn(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			static std::vector<glm::vec2> sdlVertices;
			sdlVertices.clear();
			iterate(vertices, vertexCount, [&](const glm::vec2& v) {
				sdlVertices.push_back(v);
			});
			graphic.addPolygon(sdlVertices.begin(), sdlVertices.end(), toColor(color));
			drawPolygonFcn(vertices, vertexCount, color, context);
		}

		/// Draw a circle.
		void drawCircleFcn(b2Vec2 center, float radius, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			graphic.addCircleOutline({center.x, center.y}, radius, LineWidth, toColor(color));
		}

		/// Draw a solid circle.
		void drawSolidCircleFcn(b2Transform transform, float radius, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			// Transform the center of the circle.
			b2Vec2 center = transform.p;
			graphic.addCircle({center.x, center.y}, radius, toColor(color));
			// Draw the axis.
			b2Vec2 axis = b2Rot_GetXAxis(transform.q);
			graphic.addLine({center.x, center.y}, {center.x + axis.x * radius, center.y + axis.y * radius}, LineWidth, toColor(color));
		}

		/// Draw a solid capsule.
		void drawSolidCapsuleFcn(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			// Draw the capsule as a circle at each end and a line segment between them.
			graphic.addCircle({p1.x, p1.y}, radius, toColor(color));
			graphic.addCircle({p2.x, p2.y}, radius, toColor(color));
			graphic.addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, toColor(color));
		}

		/// Draw a line segment.
		void drawSegmentFcn(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			graphic.addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, toColor(color));
		}

		/// Draw a transform. Choose your own length scale.
		void drawTransformFcn(b2Transform transform, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			const float k_axisScale = 0.4f;
			b2Vec2 p1 = transform.p;
			b2Vec2 p2 = p1 + k_axisScale * b2Rot_GetXAxis(transform.q);
			graphic.addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, sdl::color::html::Red);
			p2 = p1 + k_axisScale * b2Rot_GetYAxis(transform.q);
			graphic.addLine({p1.x, p1.y}, {p2.x, p2.y}, LineWidth, sdl::color::html::Green);
		}

		/// Draw a point.
		void drawPointFcn(b2Vec2 p, float size, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			graphic.addPixel({p.x, p.y}, toColor(color), size);
		}

		/// Draw a string in world space
		void drawStringFcn(b2Vec2 p, const char* s, b2HexColor color, void* context) {
			Graphic& graphic = *static_cast<Graphic*>(context);
			// This is a placeholder implementation. You can replace it with your own string rendering logic.
			// For example, you could use a text rendering library to draw the string at the specified position.
			// graphic.addText({p.x, p.y}, s, toColor(color));
			// For now, we will just log the string.
			printf("DrawString: %s at (%f, %f)\n", s, p.x, p.y);
		}

	}

	b2DebugDraw initb2DebugDraw(Graphic& graphic) {
		b2DebugDraw dd = b2DefaultDebugDraw();
		dd.context = &graphic;
		dd.drawingBounds = b2AABB{
			.lowerBound = {-1000.f, -1000.f},
			.upperBound = {1000.f, 1000.f}
		};
		dd.DrawPolygonFcn = &drawPolygonFcn;
		dd.DrawSolidPolygonFcn = &drawSolidPolygonFcn;
		dd.DrawCircleFcn = &drawCircleFcn;
		dd.DrawSolidCircleFcn = &drawSolidCircleFcn;
		dd.DrawSolidCapsuleFcn = &drawSolidCapsuleFcn;
		dd.DrawSegmentFcn = &drawSegmentFcn;
		dd.DrawTransformFcn = &drawTransformFcn;
		dd.DrawPointFcn = &drawPointFcn;
		dd.DrawStringFcn = &drawStringFcn;
		return dd;
	}

}
