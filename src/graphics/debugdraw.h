#ifndef ZOMBIE_DEBUGDRAW_H
#define ZOMBIE_DEBUGDRAW_H

#include "graphics/debugdraw.h"
#include "../graphic.h"

#include <box2d/box2d.h>
#include <sdl/color.h>
//#include <box2d/b2_collision.h>

#include <utility>

namespace zombie {

	struct DebugDrawSettings {
		/// Bounds to use if restricting drawing to a rectangular region
		b2AABB drawingBounds;

		/// Option to restrict drawing to a rectangular region. May suffer from unstable depth sorting.
		bool useDrawingBounds;

		/// Option to draw shapes
		bool drawShapes;

		/// Option to draw joints
		bool drawJoints;

		/// Option to draw additional information for joints
		bool drawJointExtras;

		/// Option to draw the bounding boxes for shapes
		bool drawBounds;

		/// Option to draw the mass and center of mass of dynamic bodies
		bool drawMass;

		/// Option to draw body names
		bool drawBodyNames;

		/// Option to draw contact points
		bool drawContacts;

		/// Option to visualize the graph coloring used for contacts and joints
		bool drawGraphColors;

		/// Option to draw contact normals
		bool drawContactNormals;

		/// Option to draw contact normal impulses
		bool drawContactImpulses;

		/// Option to draw contact feature ids
		bool drawContactFeatures;

		/// Option to draw contact friction impulses
		bool drawFrictionImpulses;

		/// Option to draw islands as bounding boxes
		bool drawIslands;
	};

	b2DebugDraw initb2DebugDraw(Graphic& graphic, const DebugDrawSettings& settings);

}

#endif
