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

	vb=new VertexBuffer(vertices, 4*3*sizeof(float));


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

    ib=new IndexBuffer(indices, 6);

	glUseProgram(program);
	int location=glGetUniformLocation(program,"u_Color");
	glUniform4f(location,0.6,0.3,0.0,1.0);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

Mesh::~Mesh()
{
	//TODO proper deallocation
	delete vb;
	delete ib;
}


void Mesh::render()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	ib->Bind();
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