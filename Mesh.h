#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
class Mesh
{
public:
	Mesh();
	~Mesh();

	void render();

private:
	unsigned int vbo = 0;
	unsigned int vao = 0;
	unsigned int ibo = 0;
	unsigned int program = 0;
	VertexBuffer* vb;
	IndexBuffer* ib;
};