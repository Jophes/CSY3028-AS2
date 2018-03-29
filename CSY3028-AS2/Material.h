#pragma once

#include "ShaderProgramme.h"
#include "Texture.h"

class Material {
public:
	Material();
	Material(ShaderProgramme* program);

	void Init();

	GLuint*  GetShaderProg();
private:
	ShaderProgramme* shaderProg;
	Texture* texture;
	Texture* normal;
};

