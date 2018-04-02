#pragma once

#include "ShaderProgramme.h"
#include "Texture.h"

class Material {
public:
	Material();
	Material(ShaderProgramme* program, Texture* texture);

	void Init();

	GLuint*  GetShaderProg();
	void SetActive();
private:
	ShaderProgramme* shaderProg;
	Texture* texture;
};

