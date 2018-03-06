#pragma once

#include <string>
#include "Mesh.h"

class Model {
public:
	Model(std::string object);
	~Model();

	std::string name;
	Mesh mesh;
};
