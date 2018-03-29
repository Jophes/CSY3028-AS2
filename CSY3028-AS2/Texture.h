#pragma once

#include <gl\glew.h>

class Texture
{
public:
	Texture();
	Texture(char* ps, int w, int h);
	Texture(const char* filename);
	~Texture();

	void Init(GLuint activeTexture);

	char* pixels;
	int width, height;

	GLuint id;
};

