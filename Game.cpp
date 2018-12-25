#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>

#define LOG(x) std::cout<<x<<std::endl;

struct ShaderSourceContainer
{
    std::string VertexSource;
    std::string FragmentSource;
};


static ShaderSourceContainer LoadShader(const std::string& filePath)
{
    std::ifstream file;
    file.open(filePath.c_str());

    enum class ShaderType
    {
        NONE=-1,VERTEX=0,FRAGMENT=1
    };


    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(file,line))
    {
        if(line.find("#shader")!=std::string::npos)
        {
            if(line.find("vertex")!=std::string::npos)
            {
                type=ShaderType::VERTEX;
            }
            else if(line.find("fragment")!=std::string::npos)
            {
                type=ShaderType::FRAGMENT;
            }

        }
        else
        {
            ss[(int)type]<<line<<'\n';
        }

    }

    return {ss[0].str(),ss[1].str()};
}



static void CheckShaderError(unsigned int shader, unsigned int flag,bool isProgram,const std::string& errorMessage)
{
    int success = 0;
    char error[1024] = {0};

    if(isProgram)
    {
        glGetProgramiv(shader,flag,&success);
    }
    else
    {
        glGetShaderiv(shader,flag,&success);
    }

    if(success == GL_FALSE)
    {
        if(isProgram)
        {
            glGetProgramInfoLog(shader,sizeof(error),NULL,error);
        }
        else
        {
            glGetShaderInfoLog(shader,sizeof(error),NULL,error);
        }

        std::cerr<<errorMessage<<" :'"<<error<<" '"<<std::endl;
    }


}
static unsigned int CompileShader(const char* source, unsigned int type)
{
    unsigned int id=glCreateShader(type);
    glShaderSource(id,1,&source,NULL);
    glCompileShader(id);
    CheckShaderError(id,GL_COMPILE_STATUS,false,"Error : shader compilation failed :");
    return id;
}

static int CreateShader(const char* vert,const char* frag)
{

    unsigned int vs=CompileShader(vert,GL_VERTEX_SHADER);
    unsigned int fs=CompileShader(frag,GL_FRAGMENT_SHADER);
    unsigned int program=glCreateProgram();
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    CheckShaderError(program,GL_LINK_STATUS,true,"Error : program linking failed :");
    glValidateProgram(program);
    CheckShaderError(program,GL_VALIDATE_STATUS,true,"Error : program is invalid :");

    return program;

}

void Game::init(const char* title)
{
    if(!SDL_Init(SDL_INIT_EVERYTHING)) //if SDL_Init() ==0
    {
        LOG("SDL initialization success....");
        LOG("Now creating window....")

        int r, g, b, a, minor, major, visual, depthSize, stencil, buffer, dblbuffer;
        int core, flags;

        
        /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
*/
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
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &core);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);

            SDL_GL_GetAttribute(SDL_GL_RED_SIZE,&r);
            SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE,&g);
            SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE,&b);
            SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE,&a);
            SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE,&buffer);
            SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER,&dblbuffer);


            std::cout<<"Attributes:"<<std::endl;
            std::cout<<"SDL_GL_CONTEXT_MAJOR_VERSION:"<<major<<std::endl;
            std::cout<<"SDL_GL_CONTEXT_MINOR_VERSION:"<<minor<<std::endl;
            std::cout<<"SDL_GL_CONTEXT_PROFILE_MASK:"<<core<<std::endl;
            std::cout<<"SDL_GL_CONTEXT_FLAGS:"<<flags<<std::endl;
            std::cout<<"GL_RED_SIZE:"<<r<<std::endl;
            std::cout<<"GL_GREEN_SIZE:"<<g<<std::endl;
            std::cout<<"GL_BLUE_SIZE:"<<b<<std::endl;
            std::cout<<"GL_ALPHA_SIZE:"<<a<<std::endl;
            std::cout<<"GL_BUFFER_SIZE:"<<buffer<<std::endl;
            std::cout<<"GL_DOUBLEBUFFER:"<<buffer<<std::endl;

            std::cout<<"OpenGL:"<<std::endl;
            std::cout << "Version " << glGetString(GL_VERSION) << std::endl;
            std::cout << "Vendor " << glGetString(GL_VENDOR) << std::endl;
            std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
            std::cout << "Shading " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
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
    //select the buffer generated
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //copy the data into the currently bound buffer.
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(float),positions,GL_STATIC_DRAW);

   /* unsigned int vao=0;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    */
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,NULL);
   
    glEnableVertexAttribArray(0);

    ShaderSourceContainer code=LoadShader("Shaders/basic.shader");
    std::cout<<"VERTEX"<<std::endl;
    std::cout<<code.VertexSource<<std::endl;
    std::cout<<"FRAGMENT"<<std::endl;
    std::cout<<code.FragmentSource<<std::endl;


    unsigned int shader = CreateShader(code.VertexSource.c_str(),code.FragmentSource.c_str());
    glUseProgram(shader);

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
