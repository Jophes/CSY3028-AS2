#include "Face.h"

// Line
Vertex::Vertex(unsigned int v) {
	this->v = v;
	mode = VertMode::Vert;
}
Vertex::Vertex(unsigned int v, unsigned int vn) {
	this->v = v;
	this->vn = vn;
	mode = VertMode::VertNorms;
}
Vertex::Vertex(unsigned int v, unsigned int vt, unsigned int vn) {
	this->v = v;
	this->vt = vt;
	this->vn = vn;
	mode = VertMode::VertNormsTexs;
}

std::string Vertex::ToString() {
	if (mode == VertMode::Vert) {
		return std::to_string(v);
	}
	else if (mode == VertMode::VertNorms) {
		return std::to_string(v) + "//" + std::to_string(vn);
	}
	else if (mode == VertMode::VertNormsTexs) {
		return std::to_string(v) + "/" + std::to_string(vn) + "/" + std::to_string(vt);
	}
}

// Face
Face::Face() {
	verts = std::vector<Vertex>();
}

Face::~Face() {
}

std::string Face::ToString() {
	std::string str = "f ";
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		str += verts[i].ToString() + (i < verts.size() - 1 ? " " : "");
	}
	return str;
}