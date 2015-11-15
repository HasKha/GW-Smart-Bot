#include <stdio.h>

#include "Application.h"

int main(int argc, char** argv) {
	
	Application app;
	bool connected = app.Connect();
	if (!connected) return EXIT_FAILURE;

	while (true) {
		app.HandleInput();
		if (app.ShouldQuit()) break;

		app.Update();
		app.Render();
	}

	app.Disconnect();

	return EXIT_SUCCESS;
}
