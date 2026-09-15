#pragma once
#ifndef BOX_H
#define BOX_H

#include "Shape.h"
#include <Vector.h>
#include <vector>
#include <Quaternion.h>

namespace GEOMETRY {
	class Box : public Shape {
	private:
		// Represent an Orientated Bounding Box by centre position, half extents, and orientation.REFERENCE : Real Time Collision Detection by Ericson
		// Ericson recommends keeping the orientation as a matrix for fast collisiondetection, but I love quaternions too much
		MATH::Vec3 centre;
		MATH::Vec3 halfExtents;
		MATH::Quaternion orientation;

	public:
		MATH::Vec3 getCentre() const;
		MATH::Vec3 getHalfExtents() const;
		MATH::Quaternion getOrientation() const;

		Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_);
		// The Set method is handy, as it ensures I generate vertices and normals
		// whenever the box is changed
		void set(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_);
		// Plus a bunch of annoying getters

		// Fill the vertices and normals list with Vec3's to represent a box
		void generateVerticesAndNormals() override;
	};
}
#endif
