#pragma once

#include <gl\glew.h>
#include <string>

class Shader {
public:
	Shader();
	Shader(const char* fileName, GLuint shaderType);
	GLuint* GetShader();
private:
	GLuint shaderObj;
	std::string ReadFile(const char* fileName);
	std::string GetShaderName(GLuint shaderType);
};