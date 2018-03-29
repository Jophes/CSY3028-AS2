#pragma once

#include <vector>
#include <string>

enum VertMode { Vert, VertNorms, VertNormsTexs, None };
class Vertex {
public:
	VertMode mode = VertMode::None;
	unsigned int v, vt, vn;
	Vertex(unsigned int v);
	Vertex(unsigned int v, unsigned int vn);
	Vertex(unsigned int v, unsigned int vt, unsigned int vn);
	std::string ToString();
};

class Face {
public:
	Face();
	~Face();
	std::vector<Vertex> verts;
	std::string ToString();
};