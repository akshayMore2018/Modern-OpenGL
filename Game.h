#pragma once

#include "SDL2/SDL.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

class Game
{

public:
	void init(const char* title);
	void event();
	void update();
	void render();
	void clean();
	inline bool shouldExit(){return m_IsRunning;}

private:
	SDL_Window* m_Window;
	SDL_GLContext glContext;
	SDL_Event m_Event;
	bool m_IsRunning;
};
