#include <Windows.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform2.hpp>

#include <gl\glew.h>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <iostream>
#include <time.h>

#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "ShaderProgramme.h"

double secs, lastSecs;
ShaderProgramme programme;
Mesh mesh;
Camera camera;
Texture texture;
Material material;
Model model;

//std::vector<Model> models;

static double getSecs(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (double)(ts.tv_sec) + (double)(ts.tv_nsec) / 1000000000L;
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
	/*for (GLuint i = 0; i < models.size(); i++)
	{
		models[i].rotation.y += deltaTime;
		models[i].Draw();
	}*/

	// Swap buffers, call redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	// Import the object cube
	std::vector<Mesh> meshes = Mesh::ImportMeshes("cube4.obj");
	if (meshes.size() > 0) {
		mesh = meshes[0];
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
	//programme = ShaderProgramme("shader.vert", "shader.frag");
	programme = ShaderProgramme("phong.vert", "phong.frag");

	// Generate objects
	//mesh.Init(&programme);

	// Create material
	texture = Texture("sun.bmp");
	texture.Init(GL_TEXTURE0);
	material = Material(&programme);

	// Create model
	model = Model(&camera, &mesh, &material);
	/*for (GLuint i = 0; i < 32; i++)
	{
		models.push_back(Model(&camera, &mesh, &material));
		models.back().position = glm::vec3((rand() % 400) / 100.0f - 2, (rand() % 400) / 100.0f - 2, (rand() % 400) / 100.0f - 2);
		models.back().scale = glm::vec3(0.1f + (rand() % 90) / 100.0f, 0.1f + (rand() % 90) / 100.0f, 0.1f + (rand() % 90) / 100.0f);

	}*/

	secs = getSecs();
	lastSecs = secs;

	// Start glut loop
	glutMainLoop(); 

	// When loop ends, end program
	return EXIT_SUCCESS;
}