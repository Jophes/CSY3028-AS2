#pragma once

#include "ShaderProgramme.h"
#include "Texture.h"

class Material {
public:
	Material();
	Material(ShaderProgramme* program);
	Material(ShaderProgramme* program, Texture* texture);

	void Init();

	GLuint*  GetShaderProg();
	void SetActive();
private:
	bool textureSet = false;
	ShaderProgramme* shaderProg;
	Texture* texture;
};

