#include "ShaderProgramme.h"

#include <iostream>

ShaderProgramme::ShaderProgramme() {

}

ShaderProgramme::ShaderProgramme(const char* vertName, const char* fragName) {
	vertex = new Shader(vertName, ShaderType::VertexShader);
	fragment = new Shader(fragName, ShaderType::FragmentShader);

	programme = glCreateProgram();
	if (programme == 0) {
		std::cout << "Failed to create Shader Programme" << std::endl;
		exit(1);
	}

	glAttachShader(programme, *vertex->GetShader());
	glAttachShader(programme, *fragment->GetShader());

	glLinkProgram(programme);

	GLint status;
	glGetProgramiv(programme, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		// Print what failed
		std::cout << "Shader Programme failed" << std::endl;

		// Print exact error
		GLint logLen;
		glGetProgramiv(programme, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(programme, logLen, &written, log);
			std::cout << "programme log: " << log << std::endl;
			free(log);
		}
	}
	else {
		glUseProgram(programme);
		std::cout << "Shader Programme linked" << std::endl;
	}
}