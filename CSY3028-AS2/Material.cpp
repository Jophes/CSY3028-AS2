#include "Material.h"

Material::Material() {

}

Material::Material(ShaderProgramme* program) {
	shaderProg = program;
}
Material::Material(ShaderProgramme* program, Texture* tex) {
	shaderProg = program;
	texture = tex;
	textureSet = true;
}

GLuint* Material::GetShaderProg() {
	return &shaderProg->programme;
}

void Material::SetActive() {
	if (textureSet) {
		glUniform1i(glGetUniformLocation(shaderProg->programme, "Tex"), texture->id - 1);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProg->programme, "Tex"), 0);
	}
}

void Material::Init() {
	
}