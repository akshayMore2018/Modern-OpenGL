#include <iostream>
#include "Application.h"
#define LOG_VB(x,y) std::cout << x << y << std::endl;
#define LOG(x) std::cout << x << std::endl;


void Application::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Throne", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		LOG("Failed to create GLFW window");
		glfwTerminate();

	}

	glfwMakeContextCurrent(window);
	LOG("OpenGL:");
    LOG_VB("Version :", glGetString(GL_VERSION));
    LOG_VB("Vendor :", glGetString(GL_VENDOR));
    LOG_VB("Renderer :", glGetString(GL_RENDERER));
	LOG_VB("Shading :", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		LOG("Failed to initialize GLEW");
	}

	glViewport(0, 0, screenWidth, screenHeight);
	mesh = new Mesh();
}

void Application::events()
{
	glfwPollEvents();
}

void Application::render()
{
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	mesh->render();
	glfwSwapBuffers(window);


}

void Application::clean()
{

	glfwTerminate();
}

bool Application::running()
{
	return !glfwWindowShouldClose(window);
}
