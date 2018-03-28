
#include <Windows.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform2.hpp>
#include <gl\glew.h>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <time.h>

#include "Camera.h"
#include "Model.h"
#include "ShaderProgramme.h"

double secs, lastSecs;
ShaderProgramme programme;
Mesh mesh;
Camera camera;
Material material;
Model model;
//glm::mat4 viewMatrix, projMatrix;

static double getSecs(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (double)(ts.tv_sec) + (double)(ts.tv_nsec) / 1000000000L;
}

std::vector<Mesh> importObj(std::string fileDir) {
	std::vector<Mesh> meshes;
	std::ifstream file(fileDir);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::vector<std::string> result{ std::istream_iterator<std::string>(iss),{} };
		if ((int)result.size() > 0) {
			if (result[0] == "#") {
				//std::cout << "# ";
				/*for (unsigned int i = 1; i < result.size(); i++) {
					std::cout << result[i] + (i < result.size() - 1 ? " " : "");
				}*/
			}
			else if (result[0] == "o") {
				meshes.push_back(Mesh());
				meshes.back().name = result[1];
				//std::cout << "o " + meshes.back().name;
			}
			else if (result[0] == "v") {
				meshes.back().vertices.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				//std::cout << "v " + meshes.back().vertices.back().ToString();
			}
			else if (result[0] == "vn") {
				meshes.back().normals.push_back(Vec3(atof(result[1].c_str()), atof(result[2].c_str()), atof(result[3].c_str())));
				//std::cout << "vn " + meshes.back().normals.back().ToString();
			}
			else if (result[0] == "s") {
				//std::cout << "s " + result[1];
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
					meshes.back().faces.back().lines.push_back(Line((unsigned int)atof(seglist[0].c_str()) - 1, (unsigned int)atof(seglist[1].c_str()) - 1));
				}
				//std::cout << meshes.back().faces.back().ToString();
			}
		}
		//std::cout << std::endl;
	}
	return meshes;
}

void reshape(int width, int height) {
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);

	camera.proj = glm::perspective(45.0f, (float)width / height, 3.0f, 100.0f);
	camera.view = glm::lookAt(glm::vec3(0.0, 0.5, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void display() {
	lastSecs = secs;
	secs = getSecs();
	double deltaTime = (secs - lastSecs);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model.rotation.y += deltaTime;
	model.Draw();

	// Swap buffers, call redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	// Import the object cube
	std::vector<Mesh> meshes = importObj("monkey1.obj");
	if (meshes.size() > 0) {
		mesh = meshes.front();
	}

	// Setup glut
	glutInit(&argc, argv);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("CSY3028 - Graphics 2 - Assignment 2");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Setup glew
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error initializing glew:  " << glewGetErrorString(err) << std::endl;
	}
	
	// Initialize GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnableClientState(GL_VERTEX_ARRAY);
	
	// Compile shaders
	programme = ShaderProgramme("shader.vert", "shader.frag");

	// Generate objects
	mesh.Init();

	// Create material
	material = Material(&programme);

	// Create material
	model = Model(&camera, &mesh, &material);

	secs = getSecs();
	lastSecs = secs;

	// Start glut loop
	glutMainLoop(); 

	// When loop ends, end program
	return EXIT_SUCCESS;
}