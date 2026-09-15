#include "Capsule.h"
#include <MMath.h>
#include <assert.h>

using namespace MATH;
using namespace GEOMETRY;

Capsule::Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_) {
	set(r_, sphereCentrePosA_, sphereCentrePosB_);
}

void Capsule::set(float r_, Vec3 sphereCentrePosA_, Vec3 sphereCentrePosB_) {
	assert(r_ > VERY_SMALL); // Blow up if the radius is zero or negative
	r = r_;
	sphereCentrePosA = sphereCentrePosA_;
	sphereCentrePosB = sphereCentrePosB_;
	// Generate vertices and normals whenever the radius or centre position is changed
	generateVerticesAndNormals();
}

float Capsule::getRadius() const { return r; }
Vec3 Capsule::getCentre1() const { return sphereCentrePosA; }
Vec3 Capsule::getCentre2() const { return sphereCentrePosB; }

void Capsule::generateVerticesAndNormals()
{
	Vec3 normal = VMath::normalize(sphereCentrePosB - sphereCentrePosA);

	// We need to fill the vertices and normals arrays with the correct data for a Capsule
	// deltaTheta governs how many points per ring. Try messing with it
	const float deltaTheta = 2.0f;
	// deltaPhi governs how many rings there are in total. Try messing with it
	const float deltaPhi = 2.0f;

	// deltaTheta governs how many points per ring. Try messing with it
	const float circleDeltaTheta = 0.07f;
	// deltaPhi governs how many rings there are in total. Try messing with it
	const float circleDeltaPhi = 0.07f;

	Vec3 axis = sphereCentrePosB - sphereCentrePosA;
	float height = VMath::mag(axis);

	for (float thetaDeg = -90.0f; thetaDeg <= 90.0f; thetaDeg += deltaTheta)
	{
		// Build a ring
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);

		
		for (float phiDeg = 0.0f; phiDeg <= 180.0f; phiDeg += deltaPhi) {
			// Rotate a point in the ring around the y-axis to build a sphere!
			Matrix3 rotationMatrix = MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			// Push the circle point to our vertices array, but offset by the circle centre
			vertices.push_back(sphereCentrePosA + circle);
			// The normal of a sphere points outwards to the ring
			normals.push_back(VMath::normalize(circle));
		}
	}

	for (float thetaDeg = 90.0f; thetaDeg <= 270.0f; thetaDeg += deltaTheta)
	{
		// Build a ring
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);


		for (float phiDeg = 0.0f; phiDeg <= 180.0f; phiDeg += deltaPhi) {
			// Rotate a point in the ring around the y-axis to build a sphere!
			Matrix3 rotationMatrix = MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			// Push the circle point to our vertices array, but offset by the circle centre
			vertices.push_back(sphereCentrePosB + circle);
			// The normal of a sphere points outwards to the ring
			normals.push_back(VMath::normalize(circle));
		}
	}

	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += circleDeltaTheta)
	{
		// Build a ring
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), 0.0f, r * cos(thetaDeg * DEGREES_TO_RADIANS));
		for (float h = 0.0f; h <= height; h += circleDeltaPhi) {
			// Rotate a point in the ring around the y-axis to build a Capsule!
			Vec3 position = sphereCentrePosB + Vec3(0.0f, h, 0.0f);
			// Push the circle point to our vertices array, but offset by the circle centre
			vertices.push_back(position + circle);
			// The normal of a Capsule points outwards to the ring
			normals.push_back(VMath::normalize(circle));
		}
	}

	// Once we are all done filling the vertices and normals, use the base class method to store the data in the GPU
	storeMeshData(GL_POINTS);
}