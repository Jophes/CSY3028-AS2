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
#include "LightSource.h"

double secs, lastSecs;
ShaderProgramme programme;
Mesh mesh, torus;
Camera camera;
Texture brick = Texture("brick.bmp"), sun = Texture("sun.bmp"), car = Texture("car.bmp");

LightSource* light;

std::vector<Material> materials;
//Model model;

std::vector<Model> models;

static double getSecs(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (double)(ts.tv_sec) + (double)(ts.tv_nsec) / 1000000000L;
}

void reshape(int width, int height) {
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);

	camera.proj = glm::perspective(45.0f, (float)width / height, 3.0f, 100.0f);
	camera.view = glm::lookAt(glm::vec3(0.0, 0, 6.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

}

GLfloat camTimer = 0;

void display() {
	lastSecs = secs;
	secs = getSecs();
	double deltaTime = (secs - lastSecs);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//camTimer += deltaTime * 0.5f;
	//camera.view = glm::lookAt(glm::vec3(sin(camTimer) * 6.0f, 0.0, cos(camTimer) * 6.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	// USE VIEW UNIFORM AGAIN!!

	/*model.rotation.y += deltaTime;
	model.Draw();*/
	for (GLuint i = 0; i < models.size(); i++)
	{
		//models[i].rotation.x += deltaTime;
		models[i].rotation.y += deltaTime;
		models[i].Draw();
	}

	// Swap buffers, call redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	// Import the object cube
	std::vector<Mesh> meshes = Mesh::ImportMeshes("car.obj");
	if (meshes.size() > 0) {
		mesh = meshes[0];
	}

	//torus = Mesh::GenerateTorus(4, 6, 0.7f, 0.3f);
	torus = Mesh::GenerateTorus(24, 48, 0.575f, 0.25f);
	//torus = Mesh::GenerateTorus(512, 512, 0.7f, 0.3f);

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
	
	// Compile shaders
	programme = ShaderProgramme("phong.vert", "phong.frag");

	glm::vec3 pos = glm::vec3(10.0, 0.0, 0.0), ambient = glm::vec3(0.2, 0.2, 0.2), diffuse = glm::vec3(1.0, 1.0, 1.0) * 1.5f, specular = glm::vec3(0.7, 0.7, 0.7);
	light = new LightSource(&programme, &pos, &ambient, &diffuse, &specular);
	
	// Generate objects
	mesh.Init(&programme);
	torus.Init(&programme);

	// Create material
	car.Init(GL_TEXTURE0);
	brick.Init(GL_TEXTURE1);
	sun.Init(GL_TEXTURE2);

	materials.push_back(Material(&programme, &car));
	materials.push_back(Material(&programme, &brick));
	materials.push_back(Material(&programme, &sun));

	// Create model
	models.push_back(Model(&camera, &mesh, &materials[0]));
	models.back().scale = glm::vec3(1.2, 1.2, 1.2);

	for (GLuint i = 0; i < 9; i++)
	{
		if (models.size() <= i) {
			models.push_back(Model(&camera, &torus, &(i < materials.size() ? materials[i] : materials.back())));
			models.back().scale = glm::vec3(1.0, 1.0, 1.0);
			models.back().rotation = glm::vec3(3.1459 * 0.5, 0, 0);
		}
		models.back().position = glm::vec3((i % 3) * 2.0 - 2.0, floor(i / 3) * 2.0 - 2.0, 0);
	}

	secs = getSecs();
	lastSecs = secs;

	// Start glut loop
	glutMainLoop(); 

	// When loop ends, end program
	return EXIT_SUCCESS;
}