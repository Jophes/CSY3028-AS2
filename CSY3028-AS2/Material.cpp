#include "Material.h"

Material::Material() {

}

Material::Material(ShaderProgramme* program, Texture* tex) {
	shaderProg = program;
	texture = tex;
}

GLuint* Material::GetShaderProg() {
	return &shaderProg->programme;
}

void Material::SetActive() {
	glUniform1i(glGetUniformLocation(shaderProg->programme, "Tex"), texture->id - 1);
}

void Material::Init() {
	
}