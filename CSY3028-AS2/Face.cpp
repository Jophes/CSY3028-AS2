#include "Face.h"

// Line
Line::Line(unsigned int _id0, unsigned int _id1) {
	id0 = _id0;
	id1 = _id1;
}

std::string Line::ToString() {
	return std::to_string(id0 + 1) + "//" + std::to_string(id1 + 1);
}

// Face
Face::Face() {
	lines = std::vector<Line>();
}

Face::~Face() {
}

std::string Face::ToString() {
	std::string str = "f ";
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		str += lines[i].ToString() + (i < lines.size() - 1 ? " " : "");
	}
	return str;
}