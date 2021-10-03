//Author: Mikael Cedergren

#include "main.h"

int main(int argc, char* argv[]) {
	//Reserve memory for one instance of Engine.
	memset(&engine, 0, sizeof(Engine));

	//Reserve memory for one instance of Engine.
	memset(&input, 0, sizeof(Input));

	//Init engine and game
	InitSDL();
	InitSounds();
	InitGame();

	atexit(CleanUp);

	while (1) {
		InitRenderer();
		HandleInput();

		engine.delegate.update();
		engine.delegate.draw();

		DrawRenderer();

		//Cap frame rate
		//I'm not using delta time because it's a small game.
		float framerate = 1/(float)FPS * 1000;
		SDL_Delay(framerate);
	}

	return 0;
}
