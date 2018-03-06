#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glut.h>
#include <iostream>

#include <vector>
#include <fstream>
#include "Model.h"
#include <sstream>
#include <iterator>

void importObj(std::string fileDir) {
	std::vector<Model> models;
	std::ifstream file("cube.obj");
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::vector<std::string> result{ std::istream_iterator<std::string>(iss),{} };
		if ((int)result.size() > 0) {
			if (result[0] == "#") {
				std::cout << "# ";
				for (unsigned int i = 1; i < result.size(); i++) {
					std::cout << result[i] + (i < result.size() - 1 ? " " : "");
				}
			}
			else if (result[0] == "o") {
				models.push_back(Model(result[1]));
				std::cout << "o " + models.back().name;
			}
			else if (result[0] == "v") {
				models.back().mesh.vertices.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				std::cout << "v " + models.back().mesh.vertices.back().ToString();
			}
			else if (result[0] == "vn") {
				models.back().mesh.normals.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				std::cout << "vn " + models.back().mesh.normals.back().ToString();
			}
			else if (result[0] == "s") {
				std::cout << "s " + result[1];
			}
			else if (result[0] == "f") {
				models.back().mesh.faces.push_back(Face());
				for (unsigned int i = 1; i < result.size(); i++) {
					std::stringstream test(result[i]);
					std::string segment;
					std::vector<std::string> seglist;

					while (std::getline(test, segment, '/')) {
						if (segment != "") {
							seglist.push_back(segment);
						}
					}
					models.back().mesh.faces.back().lines.push_back(Line((unsigned int)atof(seglist[0].c_str()) - 1, (unsigned int)atof(seglist[1].c_str()) - 1));
				}
				std::cout << models.back().mesh.faces.back().ToString();
			}
		}
		std::cout << std::endl;
	}

	int intest = 0;
	std::cin >> intest;
	//return nullptr;
}

int main(int argc, char **argv) {

	importObj("cube.obj");

	int test = 0;
	std::cin >> test;

	return EXIT_SUCCESS;
}