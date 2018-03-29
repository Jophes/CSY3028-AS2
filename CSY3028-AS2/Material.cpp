#include "Material.h"

Material::Material() {

}

Material::Material(ShaderProgramme* program) {
	shaderProg = program;
}

GLuint* Material::GetShaderProg() {
	return &shaderProg->programme;
}

void Material::Init() {
	
}