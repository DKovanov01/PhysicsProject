#include "Cylinder.h"
#include <MMath.h>
#include <assert.h>

using namespace MATH;
using namespace GEOMETRY;

Cylinder::Cylinder(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_) {
	set(r_, capCentrePosA_, capCentrePosB_);
}

void Cylinder::set(float r_, Vec3 capCentrePosA_, Vec3 capCentrePosB_) {
	assert(r_ > VERY_SMALL); // Blow up if the radius is zero or negative
	r = r_;
	capCentrePosA = capCentrePosA_;
	capCentrePosB = capCentrePosB_;
	// Generate vertices and normals whenever the radius or centre position is changed
	generateVerticesAndNormals();
}

float Cylinder::getRadius() const { return r; }
Vec3 Cylinder::getCentre1() const { return capCentrePosA; }
Vec3 Cylinder::getCentre2() const { return capCentrePosB; }

void Cylinder::generateVerticesAndNormals()
{
	// We need to fill the vertices and normals arrays with the correct data for a Cylinder
	// deltaTheta governs how many points per ring. Try messing with it
	const float deltaTheta = 1.0f;
	// deltaPhi governs how many rings there are in total. Try messing with it
	const float deltaPhi = 0.1f;

	Vec3 axis = capCentrePosA - capCentrePosB;
	float height = VMath::mag(axis);

	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
	{
		// Build a ring
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
		for (float h = 0.0f; h <= height; h += deltaPhi) {
			// Rotate a point in the ring around the y-axis to build a Cylinder!
			Vec3 position = capCentrePosB + Vec3(0.0f, 0.0f, h);
			// Push the circle point to our vertices array, but offset by the circle centre
			vertices.push_back(position + circle);
			// The normal of a Cylinder points outwards to the ring
			normals.push_back(VMath::normalize(circle));
		}
	}

	// Once we are all done filling the vertices and normals, use the base class method to store the data in the GPU
	storeMeshData(GL_POINTS);
}