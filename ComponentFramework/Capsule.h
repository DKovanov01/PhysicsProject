#pragma once
#ifndef CAPSULE_H
#define CAPSULE_H

#include "Shape.h"
#include <Vector.h>
#include <vector>

namespace GEOMETRY {
	class Capsule : public Shape {
	private:
		// Represent a Capsule by the two end cap centre positions and the radius. REFERENCE: p.195 of Real Time Collision Detection by Ericson
		// I made these private (I do listen to Scott sometimes) as I need to generate vertices and normals whenever they change
		float r;
		MATH::Vec3 sphereCentrePosA;
		MATH::Vec3 sphereCentrePosB;

	public:
		float getRadius() const;
		MATH::Vec3 getCentre1() const;
		MATH::Vec3 getCentre2() const;

		// Use the Set method in the constructor
		Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);//, MATH::Vec3 capCentrePosB_);
		// Remember to blow up if the radius is not positive and fire off generateVerticesAndNormals at the end
		void set(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);//, MATH::Vec3 capCentrePosB_);
		// Don’t forget to make getters too!
		// Fill the vertices and normals list with Vec3's to represent a Capsule
		void generateVerticesAndNormals() override;
	};
}
#endif
