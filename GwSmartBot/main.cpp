#include <stdio.h>

#include <SDL2\SDL.h>

#include "Application.h"

int main(int argc, char** argv) {
	
	Application app;
	bool connected = app.Connect();
	if (!connected) return EXIT_FAILURE;

	app.DebugFunc();

	while (true) {
		app.HandleInput();
		if (app.ShouldQuit()) break;

		app.Update();
		app.Render();

		SDL_Delay(20);
	}

	app.Disconnect();

	return EXIT_SUCCESS;
}
