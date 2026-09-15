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
	//halfExtents.x, halfExtents.y, halfExtents.z; - half extents represent distance from the center of the cube to its faces along each axis
	
	/*Calculating each vertex :
	// Front face
	-halfExtents.x, -halfExtents.y, halfExtents.z; // 0: Bottom-Left
	halfExtents.x, -halfExtents.y, halfExtents.z; // 1: Bottom-Right
	halfExtents.x, halfExtents.y, halfExtents.z; // 2: Top-Right
	-halfExtents.x, halfExtents.y, halfExtents.z;  // 3: Top-Left

	// Back face
	-halfExtents.x, -halfExtents.y, -halfExtents.z; // 4: Bottom-Left
	halfExtents.x, -halfExtents.y, -halfExtents.z; // 5: Bottom-Right
	halfExtents.x, halfExtents.y, -halfExtents.z; // 6: Top-Right
	-halfExtents.x, halfExtents.y, -halfExtents.z;  // 7: Top-Left*/

    Vec3 vertex[8];

    vertex[0] = Vec3(-halfExtents.x, -halfExtents.y, halfExtents.z);
    vertex[1] = Vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    vertex[2] = Vec3(halfExtents.x, halfExtents.y, halfExtents.z);
    vertex[3] = Vec3(-halfExtents.x, halfExtents.y, halfExtents.z);

    vertex[4] = Vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertex[5] = Vec3(halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertex[6] = Vec3(halfExtents.x, halfExtents.y, -halfExtents.z);
    vertex[7] = Vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);

    for (int i = 0; i < 8; i++)
    {
        vertex[i] = orientation * vertex[i];
        vertex[i] += centre;
    }

    auto addTriangle = [&](Vec3 a, Vec3 b, Vec3 c, Vec3 normal)
        {
            vertices.push_back(a);
            vertices.push_back(b);
            vertices.push_back(c);

            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        };


    // Calculate rotated face normals
    Vec3 frontNormal = orientation * Vec3(0.0f, 0.0f, 1.0f);
    Vec3 backNormal = orientation * Vec3(0.0f, 0.0f, -1.0f);
    Vec3 topNormal = orientation * Vec3(0.0f, 1.0f, 0.0f);
    Vec3 bottomNormal = orientation * Vec3(0.0f, -1.0f, 0.0f);
    Vec3 rightNormal = orientation * Vec3(1.0f, 0.0f, 0.0f);
    Vec3 leftNormal = orientation * Vec3(-1.0f, 0.0f, 0.0f);

    // Manually draw triangles with vertices
    // Front
    addTriangle(vertex[0], vertex[1], vertex[2], frontNormal);
    addTriangle(vertex[0], vertex[2], vertex[3], frontNormal);

    // Back
    addTriangle(vertex[4], vertex[6], vertex[5], backNormal);
    addTriangle(vertex[4], vertex[7], vertex[6], backNormal);

    // Top
    addTriangle(vertex[3], vertex[2], vertex[6], topNormal);
    addTriangle(vertex[3], vertex[6], vertex[7], topNormal);

    // Bottom
    addTriangle(vertex[0], vertex[5], vertex[1], bottomNormal);
    addTriangle(vertex[0], vertex[4], vertex[5], bottomNormal);

    // Right
    addTriangle(vertex[1], vertex[5], vertex[6], rightNormal);
    addTriangle(vertex[1], vertex[6], vertex[2], rightNormal);

    // Lleft
    addTriangle(vertex[0], vertex[3], vertex[7], leftNormal);
    addTriangle(vertex[0], vertex[7], vertex[4], leftNormal);

    storeMeshData(GL_TRIANGLES);  
}