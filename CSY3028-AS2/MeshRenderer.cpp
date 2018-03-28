#include "MeshRenderer.h"

#include <gl\glew.h>
#include <gl\glut.h>

MeshRenderer::MeshRenderer() {
}

MeshRenderer::MeshRenderer(Mesh* usableMesh) {
	mesh = usableMesh;
}

void MeshRenderer::Draw() {
}