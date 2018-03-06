#include "Mesh.h"

Mesh::Mesh() {
	vertices = std::vector<Vec3>();
	normals = std::vector<Vec3>();
	faces = std::vector<Face>();
}

Mesh::~Mesh() {
}
