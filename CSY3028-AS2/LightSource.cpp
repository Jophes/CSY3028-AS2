#include "LightSource.h"

LightSource::LightSource() {

}

LightSource::LightSource(ShaderProgramme* program, glm::vec3* position, glm::vec3* ambient, glm::vec3* diffuse, glm::vec3* specular) {
	SetPosition(program, position);
	glUniform3fv(glGetUniformLocation(program->programme, "Light.La"), 1, &(*ambient)[0]);
	glUniform3fv(glGetUniformLocation(program->programme, "Light.Ld"), 1, &(*diffuse)[0]);
	glUniform3fv(glGetUniformLocation(program->programme, "Light.Ls"), 1, &(*specular)[0]);
}

void LightSource::SetPosition(ShaderProgramme* program, glm::vec3* position) {
	glUniform3fv(glGetUniformLocation(program->programme, "Light.Position"), 1, &(*position)[0]);
}
