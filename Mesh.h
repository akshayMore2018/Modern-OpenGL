#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
class Mesh
{
public:
	Mesh();

	void render();

private:
	unsigned int vbo = 0;
	unsigned int vao = 0;
	unsigned int ibo = 0;
	unsigned int program = 0;

};