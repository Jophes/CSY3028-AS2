#include "Mesh.h"

#include <gl\glew.h>
#include <gl\glut.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>

#include <cmath>

#define M_PI       3.14159265358979323846
#define M_2_PI     6.283185307179586
#define M_PI_2     1.57079632679489661923

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
	/*GLuint valCount = vertexCount * 3;

	rgb* colVals = new rgb[vertices.size()];
	for (GLuint i = 0; i < vertices.size(); i++) {
		*(colVals + i) = hsv2rgb(i * (360.0f / vertices.size()), 1, 1);
	}

	//GLfloat* positions = new GLfloat[valCount];
	GLfloat* colours = new GLfloat[valCount];
	for (GLuint i = 0; i < faces.size(); i++) {
		for (GLuint j = 0; j < faces[i].verts.size(); j++) {
			GLuint id = i * 9 + j * 3;
			*(positions + id + 0) = vertices[faces[i].verts[j].v].x;
			*(positions + id + 1) = vertices[faces[i].verts[j].v].y;
			*(positions + id + 2) = vertices[faces[i].verts[j].v].z;

			*(colours + id + 0) = colVals[faces[i].verts[j].v].r;
			*(colours + id + 1) = colVals[faces[i].verts[j].v].g;
			*(colours + id + 2) = colVals[faces[i].verts[j].v].b;
		}
	}*/
	
	std::vector<glm::vec3> posData, normData;
	std::vector<glm::vec2> texData;
	for (GLuint i = 0; i < faces.size(); i++) {
		for (GLuint j = 0; j < faces[i].verts.size(); j++) {
			if (vertices.size() > faces[i].verts[j].v) {
				posData.push_back(vertices[faces[i].verts[j].v]);
			}
			if (normals.size() > faces[i].verts[j].vn) {
				normData.push_back(normals[faces[i].verts[j].vn]);
			}
			if (textures.size() > faces[i].verts[j].vt) {
				texData.push_back(textures[faces[i].verts[j].vt]);
			}
		}
	}
	
	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Ka"), 1, &(glm::vec3(0.1, 0.1, 0.1)[0]));
	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Kd"), 1, &(glm::vec3(0.9, 0.9, 0.9)[0]));
	glUniform3fv(glGetUniformLocation(shader->programme, "Material.Ks"), 1, &(glm::vec3(0.2, 0.2, 0.2)[0]));
	glUniform1f(glGetUniformLocation(shader->programme, "Material.Shininess"), 200.0f);

	// VBO 0
	if (posData.size() > 0) {
		glGenBuffers(1, (vertexBufferObjects + 0));
		glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 0));
		glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(glm::vec3), &posData[0], GL_STATIC_DRAW);
	}
	// VBO 1
	if (normData.size() > 0) {
		glGenBuffers(1, (vertexBufferObjects + 1));
		glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 1));
		glBufferData(GL_ARRAY_BUFFER, normData.size() * sizeof(glm::vec3), &normData[0], GL_STATIC_DRAW);
	}
	// VBO 2
	if (texData.size() > 0) {
		glGenBuffers(1, (vertexBufferObjects + 2));
		glBindBuffer(GL_ARRAY_BUFFER, *(vertexBufferObjects + 2));
		glBufferData(GL_ARRAY_BUFFER, texData.size() * sizeof(glm::vec2), &texData[0], GL_STATIC_DRAW);
	}

	// VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// 0
	if (posData.size() > 0) {
		glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexPosition"));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
		glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	}
	// 1
	if (normData.size() > 0) {
		glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexNormal"));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
		glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	}
	// 2
	if (texData.size() > 0) {
		glEnableVertexAttribArray(glGetAttribLocation(shader->programme, "VertexTexCoord"));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
		glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	}
	// VBO
	/*glGenBuffers(2, vertexBufferObjects);

	// 0
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
	//glBufferData(GL_ARRAY_BUFFER, valCount * sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(glm::vec3), &posData[0], GL_STATIC_DRAW);
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
	glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	// 1
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
	glVertexAttribPointer(glGetAttribLocation(shader->programme, "VertexColour"), 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);*/
}

GLuint fixNum(GLuint val, GLuint min, GLuint max) {
	GLfloat fixer = val % (max - min);
	if (fixer < 0) {
		return max + fixer;
	}
	else {
		return min + fixer;
	}
}

glm::vec3 crossProduct(glm::vec3 a, glm::vec3 b) {
	glm::vec3 val = glm::vec3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
	GLfloat magnitude = sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
	if (magnitude != 0) {
		val /= magnitude;
	}
	return val;
}

void Mesh::fixFace(Mesh* mesh, Face* face) {
	if (face->verts.size() >= 2) {
		mesh->normals.push_back(crossProduct(mesh->vertices[face->verts[1].v] - mesh->vertices[face->verts[0].v], mesh->vertices[face->verts[2].v] - mesh->vertices[face->verts[0].v]));
		for (GLuint i = 0; i < face->verts.size(); i++)
		{
			face->verts[i].vn = mesh->normals.size() - 1;
		}
	}
}

Mesh Mesh::GenerateTorus(GLuint stacks, GLuint slices, GLfloat radius1, GLfloat radius2) {
	Mesh torus;

	// Vertices
	torus.vertices.resize((stacks + 1) * (slices + 1));
	for (GLuint slice = 0; slice <= slices; slice++)
	{
		GLfloat curSlice = ((slice == slices ? 0 : slice) / (GLfloat)slices) * M_2_PI;
		for (GLuint stack = 0; stack <= stacks; stack++)
		{
			GLfloat curStack = ((stack == stacks ? 0 : stack) / (GLfloat)stacks) * M_2_PI;
			torus.vertices[stack + slice * (stacks + 1)] = (glm::vec3(cos(curSlice) * (radius1 + radius2 * cos(curStack)), radius2 * sin(curStack), sin(curSlice) * (radius1 + radius2 * cos(curStack))));
		}
	}

	// Normals
	torus.normals.resize(torus.vertices.size());
	for (GLuint slice = 0; slice <= slices; slice++)
	{
		GLuint curSlice = slice == slices ? 0 : slice;

		float t1 = (float)curSlice / slices * M_2_PI;
		glm::vec3 r1 = glm::vec3(cos(t1) * radius1, 0.0f, sin(t1) * radius1);

		for (GLuint stack = 0; stack <= stacks; stack++)
		{
			torus.normals[stack + slice * (stacks + 1)] = (torus.vertices[stack + slice * (stacks + 1)] - r1);
			GLfloat magnitude = sqrt(pow(torus.normals[stack + slice * (stacks + 1)].x, 2) + pow(torus.normals[stack + slice * (stacks + 1)].y, 2) + pow(torus.normals[stack + slice * (stacks + 1)].z, 2));
			if (magnitude != 0) {
				torus.normals[stack + slice * (stacks + 1)] /= magnitude;
			}
		}
	}

	// UVs
	torus.textures.resize(torus.vertices.size());
	for (GLuint slice = 0; slice <= slices; slice++)
	{
		for (GLuint stack = 0; stack <= stacks; stack++)
		{
			torus.textures[stack + slice * (stacks + 1)] = glm::vec2((float)slice / slices, (float)stack / stacks);
		}
	}

	// Triangles
	GLuint nbFaces = torus.vertices.size();
	GLuint nbTriangles = nbFaces * 2;
	GLuint nbIndexes = nbTriangles * 3;

	for (GLuint slice = 0; slice <= slices; slice++)
	{
		for (GLuint stack = 0; stack <= stacks; stack++)
		{
			int current = stack + slice * (stacks + 1);
			int next = stack + (slice < (slices) ? (slice + 1) * (stacks + 1) : 0);
			
			if (torus.faces.size() < nbTriangles)
			{
				Face face = Face();
				face.verts.push_back(Vertex(next, next, next));
				face.verts.push_back(Vertex(current, current, current));
				face.verts.push_back(Vertex(next + 1, next + 1, next + 1));
				torus.faces.push_back(face);

				face = Face();
				face.verts.push_back(Vertex(next + 1, next + 1, next + 1));
				face.verts.push_back(Vertex(current, current, current));
				face.verts.push_back(Vertex(current + 1, current + 1, current + 1));
				torus.faces.push_back(face);
			}
		}
	}
	return torus;
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
				meshes.back().normals.push_back(glm::vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				if (verboseLog) {
					//std::cout << "vn " + meshes.back().normals.back().ToString();
					std::cout << "vn " << meshes.back().normals.back().x << " " << meshes.back().normals.back().y << " " << meshes.back().normals.back().z;
				}
			}
			else if (result[0] == "vt") {
				meshes.back().textures.push_back(glm::vec2(atof(result[1].c_str()), atof(result[2].c_str())));
				if (verboseLog) {
					std::cout << "vt " << meshes.back().textures.back().x << " " << meshes.back().textures.back().y;
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

//GLfloat sliceStep = M_2_PI / slices, stackStep = M_2_PI / stacks;
//for (GLfloat u = 0; u < M_2_PI; u += sliceStep)
//{
//	for (GLfloat v = 0; v < M_2_PI; v += stackStep)
//	{
//		torus.vertices.push_back(glm::vec3(cos(u) * (innerRadius + outerRadius * cos(v)), outerRadius * sin(v), sin(u) * (innerRadius + outerRadius * cos(v))));
//		//torus.textures.push_back(glm::vec2((u / sliceStep) / (slices + 2), (v / stackStep) / (stacks + 2)));
//	}
//}
//for (GLfloat u = 0; u <= M_2_PI + sliceStep; u += sliceStep)
//{
//	for (GLfloat v = 0; v <= M_2_PI + stackStep; v += stackStep)
//	{
//		torus.textures.push_back(glm::vec2((u / sliceStep) / (slices + 1), (v / stackStep) / (stacks)));
//	}
//}
//
//for (GLuint v = 0; v < stacks; v++)
//{
//	for (GLuint u = 0; u < slices - 1; u++) {
//		Face face = Face();
//		face.verts.push_back(Vertex(u * stacks + v + 0));
//		face.verts.push_back(Vertex(u * stacks + fixNum(v + 1, 0, stacks)));
//		face.verts.push_back(Vertex(fixNum(u + 1, 0, slices) * stacks + v + 0));
//		fixFace(&torus, &face);
//		torus.faces.push_back(face);
//
//		face = Face();
//		face.verts.push_back(Vertex(u * stacks + fixNum(v + 1, 0, stacks)));
//		face.verts.push_back(Vertex(fixNum(u + 1, 0, slices) * stacks + fixNum(v + 1, 0, stacks)));
//		face.verts.push_back(Vertex(fixNum(u + 1, 0, slices) * stacks + v));
//		fixFace(&torus, &face);
//		torus.faces.push_back(face);
//	}
//}


//for (GLuint u = 1; u <= slices; u++)
//{
//	for (GLuint v = 0; v < stacks; v++)
//	{
//		// u,v -> u,v+1 -> u-1,v
//		torus.faces.push_back(Face());
//		if (v < stacks - 1) {
//			torus.faces.back().verts.push_back(Vertex(u * stacks + v,					u * (stacks + 1) + v,			0));
//			torus.faces.back().verts.push_back(Vertex(u * stacks + (v + 1),				u * (stacks + 1) + (v + 1),		0));
//			torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + v,				(u - 1) * (stacks + 1) + v,		0));

//			torus.normals.push_back(crossProduct(torus.vertices[torus.faces.back().verts[2].v] - torus.vertices[torus.faces.back().verts[0].v], torus.vertices[torus.faces.back().verts[1].v] - torus.vertices[torus.faces.back().verts[0].v]));
//			for (GLuint i = 0; i < torus.faces.back().verts.size(); i++)
//			{
//				torus.faces.back().verts[i].vn = torus.normals.size() - 1;
//			}
//		}
//		else {
//			/*std::cout << "AH";
//			if (u < slices) {
//				torus.faces.back().verts.push_back(Vertex(u * stacks + v,				u * (stacks + 1) + v,			0));
//				torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + (v + 1),	u * (stacks + 1) + (v + 1),		0));
//				torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + v,			(u - 1) * (stacks + 1) + v,		0));
//				torus.normals.push_back(crossProduct(torus.vertices[torus.faces.back().verts[2].v] - torus.vertices[torus.faces.back().verts[0].v], torus.vertices[torus.faces.back().verts[1].v] - torus.vertices[torus.faces.back().verts[0].v]));
//				for (GLuint i = 0; i < torus.faces.back().verts.size(); i++)
//				{
//					torus.faces.back().verts[i].vn = torus.normals.size() - 1;
//				}
//			}*/
//		}

//		// Generate normal for set of points
//		
//		// Apply normals to last face
//		

//		// u-1,v -> u,v+1 -> u-1,v+1
//		torus.faces.push_back(Face());
//		if (v < stacks - 1) {
//			torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + v,				(u - 1) * (stacks + 1) + v,			0));
//			torus.faces.back().verts.push_back(Vertex(u * stacks + (v + 1),				u * (stacks + 1) + (v + 1),			0));
//			torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + (v + 1),		(u - 1) * (stacks + 1) + (v + 1),	0));

//			torus.normals.push_back(crossProduct(torus.vertices[torus.faces.back().verts[2].v] - torus.vertices[torus.faces.back().verts[0].v], torus.vertices[torus.faces.back().verts[1].v] - torus.vertices[torus.faces.back().verts[0].v]));
//			for (GLuint i = 0; i < torus.faces.back().verts.size(); i++)
//			{
//				torus.faces.back().verts[i].vn = torus.normals.size() - 1;
//			}
//		}
//		else {
//			/*std::cout << "AH";
//			if (u < slices) {
//				torus.faces.back().verts.push_back(Vertex(u * stacks + v, u * (stacks + 1) + v, 0));
//				torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + (v + 1), u * (stacks + 1) + (v + 1), 0));
//				torus.faces.back().verts.push_back(Vertex((u - 1) * stacks + v, (u - 1) * (stacks + 1) + v, 0));
//				torus.normals.push_back(crossProduct(torus.vertices[torus.faces.back().verts[2].v] - torus.vertices[torus.faces.back().verts[0].v], torus.vertices[torus.faces.back().verts[1].v] - torus.vertices[torus.faces.back().verts[0].v]));
//				for (GLuint i = 0; i < torus.faces.back().verts.size(); i++)
//				{
//					torus.faces.back().verts[i].vn = torus.normals.size() - 1;
//				}
//			}*/
//		}
//		/*torus.faces.push_back(Face());
//		torus.faces.back().verts.push_back(Vertex(fixNum((u - 1) * stacks + v, 0, torus.vertices.size()), (u - 1) * stacks + v, 0));
//		torus.faces.back().verts.push_back(Vertex(fixNum(u * stacks + (v + 1), 0, torus.vertices.size()), u * stacks + (v + 1), 0));
//		torus.faces.back().verts.push_back(Vertex(fixNum((u - 1) * stacks + (v + 1), 0, torus.vertices.size()), (u - 1) * stacks + (v + 1), 0));

//		// Generate normal for set of points
//		torus.normals.push_back(crossProduct(torus.vertices[torus.faces.back().verts[2].v] - torus.vertices[torus.faces.back().verts[0].v], torus.vertices[torus.faces.back().verts[1].v] - torus.vertices[torus.faces.back().verts[0].v]));

//		// Apply normals to last face
//		for (GLuint i = 0; i < torus.faces.back().verts.size(); i++)
//		{
//			torus.faces.back().verts[i].vn = torus.normals.size() - 1;
//		}*/
//	}
//}
