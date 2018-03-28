#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 proj, view;
};

