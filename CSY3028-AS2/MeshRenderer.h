#pragma once

#include "Mesh.h"

class MeshRenderer {
public:
	MeshRenderer();
	MeshRenderer(Mesh* usableMesh);
	void Draw();
private:
	Mesh* mesh;
};

