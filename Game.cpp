#include<iostream>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>

#define LOG(x) std::cout<<x<<std::endl;


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
    SDL_GL_SwapWindow(m_Window);
}


void Game::clear(float r, float g,float b,float a)
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::clean()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

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

}
