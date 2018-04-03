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
glm::vec2 window = glm::vec2(600, 600);
ShaderProgramme programme;
Mesh mCar, mTorus, mShelf, mWalls, mLamp, mTable;
Camera camera;
Texture brick = Texture("brick.bmp"), sun = Texture("sun.bmp"), car = Texture("car.bmp"), wood = Texture("wood.bmp");
Texture curtain = Texture("curtain.bmp"), fabric = Texture("fabric.bmp"), grass = Texture("grass.bmp"), soil = Texture("soil.bmp"), sweet = Texture("sweet.bmp");
LightSource* light;
std::vector<Material> materials;
Model shelf, walls, lamp, table;
Material shelfMat, wallMat, lampMat, tableMat;
std::vector<Model> models;
int selected = -1;

static double getSecs(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (double)(ts.tv_sec) + (double)(ts.tv_nsec) / 1000000000L;
}

GLfloat fov = 45;

void reshape(int width, int height) {
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);
	
	window.x = width;
	window.y = height;

	camera.proj = glm::perspective(fov, (float)width / height, 3.0f, 100.0f);
	camera.view = glm::lookAt(glm::vec3(0.0, 0, 6.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

GLfloat camTimer = 0;
glm::vec3 restPos = glm::vec3(0, 0, 6.5f), selectedPos = glm::vec3(6.5f, 0, 6.5f);
glm::vec3 restCen = glm::vec3(0, 0, 0), selectedCen = glm::vec3(0, 0, 6.5f);
glm::vec3 curPos = restPos, curCen = restCen;

std::vector<glm::vec3> restObjects;

bool rotating = false, zooming = false; 

void display() {
	lastSecs = secs;
	secs = getSecs();
	double deltaTime = (secs - lastSecs);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//camTimer += deltaTime * 0.5f;
	//camera.view = glm::lookAt(glm::vec3(sin(camTimer) * 6.0f, 0.0, cos(camTimer) * 6.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	curPos = curPos + ((selected >= 0 ? selectedPos : restPos) - curPos) * 0.1f;
	curCen = curCen + ((selected >= 0 ? selectedCen : restCen) - curCen) * 0.1f;
	camera.view = glm::lookAt(curPos, curCen, glm::vec3(0.0, 1.0, 0.0));

	walls.Draw();
	shelf.Draw();
	lamp.Draw();
	table.Draw();

	for (GLuint i = 0; i < models.size(); i++)
	{
		//models[i].rotation.x += deltaTime;
		//models[i].rotation.y += deltaTime;
		models[i].position = models[i].position + ((selected == i ? selectedCen : glm::vec3((i % 3) * 2.0 - 2.0, floor(i / 3) * 2.0 - 2.0, 0)) - models[i].position) * 0.1f;
		models[i].Draw();
	}

	// Swap buffers, call redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}


glm::vec2 mPos = glm::vec2(0, 0);
void mouse(int button, int state, int x, int y) {
	mPos = glm::vec2(x, y);
	if (button == 0 && state == 1) {
		std::cout << "x: " << x << " y: " << y << std::endl;
		if (selected == -1) {
			glm::vec2 selectPos = glm::vec2(x, y) - window * 0.5f;
			selectPos = (selectPos + glm::vec2(window.y, window.y) * 0.5f) / (window.y / 3);
			if (selectPos.x >= 0 && selectPos.x <= 3) {
				selected = (int)(selectPos.x) + 3 * (int)(3 - selectPos.y);
				std::cout << "selected: " << selected << std::endl;
			}
		}
		else {
			selected = -1;
		}
	}
	if (button == 2) {
		rotating = (state == 0);
	}
	if (button == 1) {
		zooming = (state == 0);
	}
}

void motion(int x, int y) {
	glm::vec2 newPos = glm::vec2(x, y);
	glm::vec2 delta = mPos - newPos;
	if (selected >= 0 && selected < models.size()) {
		if (rotating) {
			models[selected].rotation.z += delta.x * 0.05f;
			models[selected].rotation.y += delta.y * 0.05f;
		}
		if (zooming) {
			models[selected].scale.x -= delta.y * 0.05f;
			models[selected].scale.y -= delta.y * 0.05f;
			models[selected].scale.z -= delta.y * 0.05f;
		}
	}
	mPos = glm::vec2(x, y);
}

int main(int argc, char **argv) {
	// Import the object cube
	std::vector<Mesh> meshes = Mesh::ImportMeshes("car.obj");
	if (meshes.size() > 0) {
		mCar = meshes[0];
	}

	meshes = Mesh::ImportMeshes("shelf.obj");
	if (meshes.size() > 0) {
		mShelf = meshes[0];
	}

	meshes = Mesh::ImportMeshes("walls.obj");
	if (meshes.size() > 0) {
		mWalls = meshes[0];
	}

	meshes = Mesh::ImportMeshes("lamp.obj");
	if (meshes.size() > 0) {
		mLamp = meshes[0];
	}

	meshes = Mesh::ImportMeshes("table.obj");
	if (meshes.size() > 0) {
		mTable = meshes[0];
	}

	//torus = Mesh::GenerateTorus(4, 6, 0.7f, 0.3f);
	mTorus = Mesh::GenerateTorus(24, 48, 0.575f, 0.25f);
	//torus = Mesh::GenerateTorus(512, 512, 0.7f, 0.3f);

	// Setup glut
	glutInit(&argc, argv);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(window.x, window.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("CSY3028 - Graphics 2 - Assignment 2");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Setup glew
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error initializing glew:  " << glewGetErrorString(err) << std::endl;
	}
	
	// Initialize GL settings
	glEnable(GL_DEPTH_TEST);
	
	// Compile shaders
	programme = ShaderProgramme("phong.vert", "phong.frag");

	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), ambient = glm::vec3(0.2, 0.2, 0.2), diffuse = glm::vec3(1.0, 1.0, 1.0) * 1.5f, specular = glm::vec3(0.7, 0.7, 0.7);
	light = new LightSource(&programme, &pos, &ambient, &diffuse, &specular);
	
	// Generate objects
	mCar.Init(&programme);
	mTorus.Init(&programme);
	mShelf.Init(&programme);
	mWalls.Init(&programme);
	mLamp.Init(&programme);
	mTable.Init(&programme);

	// Create material
	car.Init(GL_TEXTURE0);
	brick.Init(GL_TEXTURE1);
	sun.Init(GL_TEXTURE2);
	wood.Init(GL_TEXTURE3);
	curtain.Init(GL_TEXTURE4);
	fabric.Init(GL_TEXTURE5);
	grass.Init(GL_TEXTURE6);
	soil.Init(GL_TEXTURE7);

	materials.push_back(Material(&programme, &car));
	materials.push_back(Material(&programme, &brick));
	materials.push_back(Material(&programme, &sun));
	materials.push_back(Material(&programme, &wood));
	materials.push_back(Material(&programme, &curtain));
	materials.push_back(Material(&programme, &fabric));
	materials.push_back(Material(&programme, &grass));
	materials.push_back(Material(&programme, &soil));

	//curtain = Texture("curtain.bmp"), fabric = Texture("fabric.bmp"), grass = Texture("grass.bmp"), soil = Texture("soil.bmp"), sweet

	shelfMat = Material(&programme);
	shelf = Model(&camera, &mShelf, &shelfMat);
	shelf.rotation = glm::vec3(0, 3.1459 * 1.0, 0);

	wallMat = Material(&programme);
	walls = Model(&camera, &mWalls, &wallMat);
	walls.position = glm::vec3(0, 10-3.1, 6.5);

	lampMat = Material(&programme);
	lamp = Model(&camera, &mLamp, &lampMat);
	lamp.position = glm::vec3(0, 2.25, 6.5);

	table = Model(&camera, &mTable, &(materials[3]));
	table.position = glm::vec3(-5, -3.1, 6.5);
	table.rotation = glm::vec3(0, 3.1459 * 0.5, 0);

	// Create model
	models.push_back(Model(&camera, &mCar, &materials[0]));
	models.back().scale = glm::vec3(1.2, 1.2, 1.2);

	for (GLuint i = 0; i < 9; i++)
	{
		if (models.size() <= i) {
			models.push_back(Model(&camera, &mTorus, &(materials[i % materials.size()])));
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