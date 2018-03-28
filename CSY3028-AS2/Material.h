#pragma once

#include "ShaderProgramme.h"

class Material {
public:
	Material();
	Material(ShaderProgramme* program);

	ShaderProgramme* shaderProg;
};

