#include "Mesh.h"

#include <gl\glew.h>
#include <gl\glut.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>

Mesh::Mesh() {
	/*vertices = std::vector<Vec3>();
	normals = std::vector<Vec3>();
	faces = std::vector<Face>();*/
	vertices = std::vector<glm::vec3>();
	normals = std::vector<glm::vec3>();
	textures = std::vector<glm::vec2>();
	faces = std::vector<Face>();
}

typedef struct {
	double r;
	double g;
	double b;
} rgb;

rgb hsv2rgb(double h, double s, double v) {
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = v;
		out.g = v;
		out.b = v;
		return out;
	}
	hh = h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * ff));
	t = v * (1.0 - (s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = v;
		break;
	case 5:
	default:
		out.r = v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

void Mesh::Init(ShaderProgramme* shader) {
	vertexCount = faces.size() * 3;
	GLuint valCount = vertexCount * 3;

	/*rgb* colVals = new rgb[vertices.size()];
	for (GLuint i = 0; i < vertices.size(); i++) {
		*(colVals + i) = hsv2rgb(i * (360.0f / vertices.size()), 1, 1);
	}*/

	/*GLfloat* positions = new GLfloat[valCount];
	GLfloat* colours = new GLfloat[valCount];
	for (GLuint i = 0; i < faces.size(); i++) {
		for (GLuint j = 0; j < faces[i].lines.size(); j++) {
			GLuint id = i * 9 + j * 3;
			*(positions + id + 0) = vertices[faces[i].lines[j].id0].x;
			*(positions + id + 1) = vertices[faces[i].lines[j].id0].y;
			*(positions + id + 2) = vertices[faces[i].lines[j].id0].z;

			*(colours + id + 0) = colVals[faces[i].lines[j].id0].r;
			*(colours + id + 1) = colVals[faces[i].lines[j].id0].g;
			*(colours + id + 2) = colVals[faces[i].lines[j].id0].b;
		}
	}*/

	std::vector<glm::vec3> posData, normData;
	std::vector<glm::vec2> texData;
	for (GLuint i = 0; i < faces.size(); i++) {
		for (GLuint j = 0; j < faces[i].verts.size(); j++) {
			posData.push_back(vertices[faces[i].verts[j].v]);
			normData.push_back(normals[faces[i].verts[j].vn]);
			texData.push_back(textures[faces[i].verts[j].vt]);
		}
	}

	// VBO 0
	glGenBuffers(1, (vertexBufferObjects + 0));
	glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 0));
	glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(glm::vec3), &posData, GL_STATIC_DRAW);
	// VBO 1
	glGenBuffers(1, (vertexBufferObjects + 1));
	glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 1));
	glBufferData(GL_ARRAY_BUFFER, normData.size() * sizeof(glm::vec3), &normData, GL_STATIC_DRAW);
	// VBO 2
	glGenBuffers(1, (vertexBufferObjects + 2));
	glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 2));
	glBufferData(GL_ARRAY_BUFFER, texData.size() * sizeof(glm::vec2), &texData, GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// 0
	glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexPosition"));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
	glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// 1
	glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexNormal"));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
	glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// 2
	glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexTexCoord"));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
	glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Ka"), 1, &(glm::vec3(0.3, 0.3, 0.3)[0]));
	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Kd"), 1, &(glm::vec3(0.9, 0.9, 0.9)[0]));
	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Ks"), 1, &(glm::vec3(0.8, 0.8, 0.8)[0]));
	glUniform1f(glGetUniformLocation(shader->programme, "Material.Ka"), 250.0f);

	/*
	// VBO
	glGenBuffers(2, vertexBufferObjects);

	// 0
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, valCount * sizeof(GLfloat), positions, GL_STATIC_DRAW);
	// 1
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, valCount * sizeof(GLfloat), colours, GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// 0
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// 1
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);*/
}

std::vector<Mesh> Mesh::ImportMeshes(std::string fileDir, bool verboseLog) {
	std::vector<Mesh> meshes;
	std::ifstream file(fileDir);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::vector<std::string> result{ std::istream_iterator<std::string>(iss),{} };
		if ((int)result.size() > 0) {
			if (result[0] == "#") {
				if (verboseLog) {
					std::cout << "# ";
					for (unsigned int i = 1; i < result.size(); i++) {
					std::cout << result[i] + (i < result.size() - 1 ? " " : "");
					}
				}
			}
			else if (result[0] == "o") {
				meshes.push_back(Mesh());
				meshes.back().name = result[1];
				if (verboseLog) {
					std::cout << "o " + meshes.back().name;
				}
			}
			else if (result[0] == "v") {
				//meshes.back().vertices.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				meshes.back().vertices.push_back(glm::vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));

				if (verboseLog) {
					std::cout << "v " << meshes.back().vertices.back().x << " " << meshes.back().vertices.back().y << " " << meshes.back().vertices.back().z;
				}
			}
			else if (result[0] == "vn") {
				//meshes.back().normals.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				meshes.back().vertices.push_back(glm::vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				if (verboseLog) {
					//std::cout << "vn " + meshes.back().normals.back().ToString();
					std::cout << "v " << meshes.back().vertices.back().x << " " << meshes.back().vertices.back().y << " " << meshes.back().vertices.back().z;
				}
			}
			else if (result[0] == "s") {
				if (verboseLog) {
					std::cout << "s " + result[1];
				}
			}
			else if (result[0] == "f") {
				meshes.back().faces.push_back(Face());
				for (unsigned int i = 1; i < result.size(); i++) {
					std::stringstream test(result[i]);
					std::string segment;
					std::vector<std::string> seglist;

					while (std::getline(test, segment, '/')) {
						if (segment != "") {
							seglist.push_back(segment);
						}
					}
					if (seglist.size() == 1) {
						meshes.back().faces.back().verts.push_back(Vertex((unsigned int)atof(seglist[0].c_str()) - 1));
					}
					else if (seglist.size() == 2) {
						meshes.back().faces.back().verts.push_back(Vertex((unsigned int)atof(seglist[0].c_str()) - 1, (unsigned int)atof(seglist[1].c_str()) - 1));
					}
					else if (seglist.size() == 3) {
						meshes.back().faces.back().verts.push_back(Vertex((unsigned int)atof(seglist[0].c_str()) - 1, (unsigned int)atof(seglist[1].c_str()) - 1, (unsigned int)atof(seglist[2].c_str()) - 1));
					}
				}
				if (verboseLog) {
					std::cout << meshes.back().faces.back().ToString();
				}
			}
		}
		if (verboseLog) {
			std::cout << std::endl;
		}
	}
	return meshes;
}