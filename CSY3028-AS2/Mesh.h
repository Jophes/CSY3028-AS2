#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Face.h"
#include "ShaderProgramme.h"

class Mesh {
public:
	Mesh();
	std::string name;
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> textures;
	std::vector<Face> faces;
	
	void Init(ShaderProgramme* shader);

	static std::vector<Mesh> ImportMeshes(std::string filename, bool verboseLog = false);

	GLuint vertexArrayObject, vertexCount;
private:
	GLuint bufferCount = 3;
	GLuint* vertexBufferObjects = new GLuint[bufferCount];
};