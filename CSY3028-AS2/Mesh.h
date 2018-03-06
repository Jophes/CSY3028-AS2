#pragma once

#include <vector>
#include "Vec3.h"
#include "TriangleFace.h"

class Mesh {
public:
	Mesh();
	~Mesh();

	std::vector<Vec3> vertices, normals;
	std::vector<Face> faces;
};

