#include <iostream>
#include "SDL2/SDL.h"
#include "Game.h"

int main()
{
	const int FPS=60;
	const int FRAMEDELAY=1000/FPS;
	unsigned int frameStart;
	unsigned int frameTime;
	Game game;
	
	game.init("GameWindow");

	while(game.shouldExit())
	{
		frameStart=SDL_GetTicks();
		game.clear(0.0f,0.15f,0.3f,1.0f);
		game.event();
		game.update();

		frameTime=SDL_GetTicks()-frameStart;

		if(frameTime<FRAMEDELAY)
		{
			SDL_Delay(FRAMEDELAY-frameTime);
		}

	}

	game.clean();

}

