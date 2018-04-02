#include "Shader.h"

#include <iostream>
#include <fstream>

Shader::Shader() {

}


Shader::Shader(const char* fileName, ShaderType shaderType) {
	type = shaderType;
	shaderObj = glCreateShader((type == ShaderType::VertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
	if (shaderObj == 0) {
		std::cout << "Shader failed file: " + *fileName << std::endl;
		exit(1);
	}
	
	// Import the shader data
	std::string shaderData = ReadFile(fileName);
	const GLchar* shaderCData = shaderData.c_str();
	glShaderSource(shaderObj, 1, &shaderCData, NULL);

	// Compile shader
	glCompileShader(shaderObj);

	// Check the compilation result
	GLint result;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// Print type of shader being compiled
		std::cout << GetShaderName(type) << " compilation failed" << std::endl;

		// Print out compilation error
		GLint logLen;
		glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shaderObj, logLen, &written, log);
			std::cout << "Shader log: " << log << std::endl;
			free(log);
		}
	}
	else {
		std::cout << GetShaderName(type) << " compilation succeeded: " << shaderObj  << std::endl;
	}
}

GLuint* Shader::GetShader() {
	return &shaderObj;
}

std::string Shader::ReadFile(const char* fileName) {
	std::string shader_str = "";
	std::ifstream file(fileName);
	while (file.good()) {
		std::string line;
		std::getline(file, line);
		shader_str.append(line + "\n");
	}
	file.close();
	return shader_str;
}

std::string Shader::GetShaderName(ShaderType shaderType) {
	switch (shaderType) {
	case VertexShader:
		return "Vertex shader";
		break;
	case FragmentShader:
		return "Fragment shader";
		break;
	default:
		return "Unknown shader type";
		break;
	}
}