#pragma once

#include <glm\glm.hpp>
#include "ShaderProgramme.h"

class LightSource {
public:
	LightSource();
	LightSource(ShaderProgramme* program, glm::vec3* position, glm::vec3* ambient, glm::vec3* diffuse, glm::vec3* specular);
	void SetPosition(ShaderProgramme* program, glm::vec3* position);
};

