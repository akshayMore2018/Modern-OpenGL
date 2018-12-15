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
    float positions[6]={
            -0.5f,-0.5f,  /* first vertex*/
             0.0f, 0.5f,  /* second vertex*/
             0.5f,-0.5f   /* third vertex*/
        };


    unsigned int buffer;
    //generate 1 buffer
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),positions,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,NULL);
    glEnableVertexAttribArray(0);


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
