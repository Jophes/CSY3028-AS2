#include "Model.h"
#include "Mesh.h"

Model::Model(std::string object) {
	name = object;
	mesh = Mesh();
}

Model::~Model() {
}
