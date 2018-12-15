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
		game.event();
		game.update();
		game.render();

		frameTime=SDL_GetTicks()-frameStart;

		if(frameTime<FRAMEDELAY)
		{
			SDL_Delay(FRAMEDELAY-frameTime);
		}

	}

	game.clean();

}

