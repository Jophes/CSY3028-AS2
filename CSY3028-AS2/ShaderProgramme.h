#pragma once

#include <gl\glew.h>
#include "Shader.h"

class ShaderProgramme {
public:
	ShaderProgramme();
	ShaderProgramme(const char *vertName, const char *fragName);
	GLuint programme;
private:
	Shader* vertex;
	Shader* fragment;
	GLuint vertexArrayObj;
};

