#pragma once

#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "MeshRenderer.h"
#include "Material.h"
#include "Camera.h"

class Model {
public:
	Model();
	Model(Camera* cam, Mesh* msh, Material* mat);

	Mesh* mesh;
	Material* material;

	void Draw();

	glm::vec3 position, rotation, scale;

private:
	Camera* cam;
	glm::mat4 modelMatrix;
};
