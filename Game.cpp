#include<iostream>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>

#define LOG(x) std::cout<<x<<std::endl;

void Game::init(const char* title)
{
    if(!SDL_Init(SDL_INIT_EVERYTHING)) //if SDL_Init() ==0
    {
        LOG("SDL initialization success....");
        LOG("Now creating window....")

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,8);


        m_Window=SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
        if(m_Window)//if window != 0
        {
            LOG("Window creation sucess....");
            LOG("Now creating GL context...");
            glContext=SDL_GL_CreateContext(m_Window);

            GLint status = glewInit();
            if(status==GLEW_OK)
            {
                LOG("Glew initialize success...");
                m_IsRunning=true;
            }
        }
        else
        {
            LOG("Window creation failure!")
        }
    }
    else
    {
        LOG("SDL initialization failure!");
        m_IsRunning=false;
    }


    //mesh
    float positions[9]={
             0.0f, 0.5f, 0.0f,  /* first vertex*/
             0.5f,-0.5f, 0.0f,  /* second vertex*/
            -0.5f,-0.5f, 0.0f   /* third vertex*/
        };


    unsigned int vbo=0;
    //generate 1 empty buffer
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //copy the data into the currently bound buffer.
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(float),positions,GL_STATIC_DRAW);

    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,NULL);
    glEnableVertexAttribArray(0);


    const char* vertex_shader=
    "#version 130\n"
    "in vec4 position;"
    "void main()"
    "{"
    "gl_Position = position;"
    "}";

    const char* fragment_shader=
    "#version 130\n"
    "out vec4 color;"
    "void main(){"
    "color = vec4(0.5,0.0,0.5,1.0);"
    "}";


    unsigned int vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vertex_shader,NULL);
    glCompileShader(vs);


    unsigned int fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragment_shader,NULL);
    glCompileShader(fs);


    unsigned int program=glCreateProgram();
    glAttachShader(program,vs);
    glAttachShader(program,fs);

    glLinkProgram(program);
    glUseProgram(program);
    glBindVertexArray(vao);





}
void Game::event()
{
    while(SDL_PollEvent(&m_Event))
        {
            if(m_Event.type==SDL_QUIT)
                m_IsRunning=false;
        
        }
}

void Game::update()
{
    
}


void Game::render()
{
    glClearColor(0.0f,0.15f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glDrawArrays(GL_TRIANGLES,0,3);


    SDL_GL_SwapWindow(m_Window);
}

void Game::clean()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
