#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"
class Application
{

public:
	void init();
	void events();
	void render();
	void clean();
	bool running();
private:
	const unsigned int WIDTH = 600, HEIGHT = 480;
	GLFWwindow *window=nullptr;
	Mesh *mesh=nullptr;
};