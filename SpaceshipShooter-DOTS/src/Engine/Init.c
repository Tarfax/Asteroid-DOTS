//Author: Mikael Cedergren

#include "Init.h"

void InitSDL() {
	int rendererFlags;
	int windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	engine.window = SDL_CreateWindow("SPACE SHOOTER WOO!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (engine.window == NULL)	{
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	engine.renderer = SDL_CreateRenderer(engine.window, -1, rendererFlags);

	if (engine.renderer == NULL) 	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Failed to initialize SDL Audio\n");
		exit(1);
	}

	Mix_AllocateChannels(MAX_AUDIO_CHANNELS);

	TTF_Init();

	printf("SDL Init SUCCESS\n");
}

void CleanUp() {
	SDL_DestroyRenderer(engine.renderer);
	printf("SDL_DestroyRenderer\n");
	SDL_DestroyWindow(engine.window);
	printf("SDL_DestroyWindow\n");
	SDL_Quit();
	printf("SDL_Quit\n");
}
