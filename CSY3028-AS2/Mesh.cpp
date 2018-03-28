#include "Mesh.h"

#include <gl\glew.h>
#include <gl\glut.h>

Mesh::Mesh() {
	vertices = std::vector<Vec3>();
	normals = std::vector<Vec3>();
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

void Mesh::Init() {
	vertexCount = faces.size() * 3;
	GLuint valCount = vertexCount * 3;

	rgb* colVals = new rgb[vertices.size()];
	for (GLuint i = 0; i < vertices.size(); i++) {
		*(colVals + i) = hsv2rgb(i * (360.0f / vertices.size()), 1, 1);
	}

	GLfloat* positions = new GLfloat[valCount];
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
	}

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}