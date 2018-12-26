#include <iostream>
#include "Application.h"

int main()
{

	Application app;

	app.init();
	
	// Game loop
	while (app.running())
	{
		app.events();
		app.render();		
	}

	app.clean();

	return EXIT_SUCCESS;
}

