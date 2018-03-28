#pragma once

#include <gl\glew.h>
#include <vector>
#include "Vec3.h"
#include "Face.h"

class Mesh {
public:
	Mesh();
	std::string name;
	std::vector<Vec3> vertices, normals;
	std::vector<Face> faces;

	void Init();

	GLuint vertexArrayObject, vertexCount;
private:
	GLuint vertexBufferObjects[2];
};