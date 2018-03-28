#pragma once

#include <vector>
#include <string>

class Line {
public:
	unsigned int id0, id1;
	Line(unsigned int _id0, unsigned int _id1);
	std::string ToString();
};

class Face {
public:
	Face();
	~Face();
	std::vector<Line> lines;
	std::string ToString();
};