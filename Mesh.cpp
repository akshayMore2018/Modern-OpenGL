#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <glm/glm.hpp>

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderSource LoadFile(const std::string& filepath);
static unsigned int CreateShader(const char* source, unsigned int type);


Mesh::Mesh()
{

	ShaderSource src = LoadFile("shaders/basic.shader");
	std::cout << "VERTEX" << std::endl;
	std::cout << src.vertexSource << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << src.fragmentSource << std::endl;
	unsigned int vs = CreateShader(src.vertexSource.c_str(),GL_VERTEX_SHADER);
	unsigned int fs = CreateShader(src.fragmentSource.c_str(), GL_FRAGMENT_SHADER);
	
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);



	float vertices[18] =
	{
		-0.5f,-0.5f, 0.0f, //0
		 0.5f,-0.5f, 0.0f, //1
		 0.5f, 0.5f, 0.0f, //2

		-0.5f, 0.5f, 0.0f, //3
	};

	unsigned int indices[6]=
	{
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	glUseProgram(program);
	int location=glGetUniformLocation(program,"u_Color");
	glUniform4f(location,0.2,0.3,0.0,1.0);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void Mesh::render()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,NULL);
}



static unsigned int CreateShader(const char* source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	return id;
}


static ShaderSource LoadFile(const std::string& filepath)
{
	std::ifstream file(filepath);
	std::string line;
	std::stringstream ss[2];
	enum class ShaderType
	{
		NONE=-1,VERTEX=0,FRAGMENT=1
	};

	ShaderType type = ShaderType::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment")!= std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return {ss[0].str(),ss[1].str()};
}