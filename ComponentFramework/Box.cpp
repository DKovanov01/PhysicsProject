#include "Box.h"
#include <MMath.h>
#include <assert.h>

using namespace MATH;
using namespace GEOMETRY;

Box::Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_) {
	set(centre_, halfExtents_, orientation_);
}

void Box::set(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_) {
	centre = centre_;
	halfExtents = halfExtents_;
	orientation = orientation_;
	// Generate vertices and normals whenever the radius or centre position is changed
	generateVerticesAndNormals();
}

Vec3 Box::getCentre() const { return centre; }
Vec3 Box::getHalfExtents() const { return halfExtents; }
Quaternion Box::getOrientation() const { return orientation; }

void Box::generateVerticesAndNormals()
{
	Vec3 vertex[8];
	vertex[0] = Vec3(-halfExtents.x, halfExtents.y, halfExtents.z); // 0: topLeftFront
	vertex[1] = Vec3(halfExtents.x, halfExtents.y, halfExtents.z); // 1: topRightFront
	vertex[2] = Vec3(halfExtents.x, -halfExtents.y, halfExtents.z); // 2: bottomRightFront
	vertex[3] = Vec3(-halfExtents.x, -halfExtents.y, halfExtents.z); // 3: bottomLeftFront

	vertex[4] = Vec3(-halfExtents.x, halfExtents.y, -halfExtents.z); // 4: topLeftBack
	vertex[5] = Vec3(halfExtents.x, halfExtents.y, -halfExtents.z); // 5: topRightBack
	vertex[6] = Vec3(halfExtents.x, -halfExtents.y, -halfExtents.z); // 6: bottomRightBack
	vertex[7] = Vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z); // 7: bottomLeftBack
	
	for (int i = 0; i < 8; i++) { // for less lines of code :)
		vertex[i] = QMath::rotate(vertex[i], orientation);
		vertex[i] += centre;
	}

	Vec3 rightNormal = QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation);
	Vec3 leftNormal = QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation);

	Vec3 frontNormal = QMath::rotate(Vec3(0.0f, 0.0f, 1.0f), orientation);
	Vec3 backNormal = QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation);

	Vec3 topNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation);
	Vec3 bottomNormal = QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation);

	// Front triangle 1
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[1]);
	vertices.push_back(vertex[2]);

	normals.push_back(frontNormal);
	normals.push_back(frontNormal);
	normals.push_back(frontNormal);

	// Front triangle 2
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[2]);
	vertices.push_back(vertex[3]);

	normals.push_back(frontNormal);
	normals.push_back(frontNormal);
	normals.push_back(frontNormal);

	// Back triangle 1
	vertices.push_back(vertex[4]);
	vertices.push_back(vertex[6]);
	vertices.push_back(vertex[5]);

	normals.push_back(backNormal);
	normals.push_back(backNormal);
	normals.push_back(backNormal);

	// Back triangle 2
	vertices.push_back(vertex[4]);
	vertices.push_back(vertex[7]);
	vertices.push_back(vertex[6]);

	normals.push_back(backNormal);
	normals.push_back(backNormal);
	normals.push_back(backNormal);

	// Right triangle 1
	vertices.push_back(vertex[1]);
	vertices.push_back(vertex[5]);
	vertices.push_back(vertex[6]);

	normals.push_back(rightNormal);
	normals.push_back(rightNormal);
	normals.push_back(rightNormal);

	// Right triangle 2
	vertices.push_back(vertex[1]);
	vertices.push_back(vertex[6]);
	vertices.push_back(vertex[2]);

	normals.push_back(rightNormal);
	normals.push_back(rightNormal);
	normals.push_back(rightNormal);

	// Left triangle 1
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[3]);
	vertices.push_back(vertex[7]);

	normals.push_back(leftNormal);
	normals.push_back(leftNormal);
	normals.push_back(leftNormal);

	// Left triangle 2
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[7]);
	vertices.push_back(vertex[4]);

	normals.push_back(leftNormal);
	normals.push_back(leftNormal);
	normals.push_back(leftNormal);

	// Top triangle 1
	vertices.push_back(vertex[3]);
	vertices.push_back(vertex[2]);
	vertices.push_back(vertex[6]);

	normals.push_back(topNormal);
	normals.push_back(topNormal);
	normals.push_back(topNormal);

	// Top triangle 2
	vertices.push_back(vertex[3]);
	vertices.push_back(vertex[6]);
	vertices.push_back(vertex[7]);

	normals.push_back(topNormal);
	normals.push_back(topNormal);
	normals.push_back(topNormal);

	// Bottom triangle 1
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[5]);
	vertices.push_back(vertex[1]);

	normals.push_back(bottomNormal);
	normals.push_back(bottomNormal);
	normals.push_back(bottomNormal);

	// Bottom triangle 2
	vertices.push_back(vertex[0]);
	vertices.push_back(vertex[4]);
	vertices.push_back(vertex[5]);

	normals.push_back(bottomNormal);
	normals.push_back(bottomNormal);
	normals.push_back(bottomNormal);

	storeMeshData(GL_TRIANGLES);  
}