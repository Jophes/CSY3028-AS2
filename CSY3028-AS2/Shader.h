#pragma once

#include <gl\glew.h>
#include <string>

enum ShaderType { VertexShader, FragmentShader };
class Shader {
public:
	Shader();
	Shader(const char* fileName, ShaderType shaderType);
	GLuint* GetShader();
	ShaderType type;
private:
	GLuint shaderObj;
	std::string ReadFile(const char* fileName);
	std::string GetShaderName(ShaderType shaderType);
};