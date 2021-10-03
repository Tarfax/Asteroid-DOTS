//Author: Mikael Cedergren

#include "Input.h"

void HandleKeyDown(SDL_KeyboardEvent* SDL_Event) {
	if (SDL_Event->repeat == 0 && SDL_Event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		input.keyboard[SDL_Event->keysym.scancode] = 1;
	}
}

void HandleKeyUp(SDL_KeyboardEvent* SDL_Event) {
	if (SDL_Event->repeat == 0 && SDL_Event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		input.keyboard[SDL_Event->keysym.scancode] = 0;
	}
}

void HandleInput() {
	SDL_Event SDL_Event;

	while (SDL_PollEvent(&SDL_Event)) {
		switch (SDL_Event.type) {
			case SDL_KEYDOWN:
				HandleKeyDown(&SDL_Event.key);
				break;
			case SDL_KEYUP:
				HandleKeyUp(&SDL_Event.key);
				break;
			case SDL_QUIT:
				exit(0);
				break;

			default:
				break;
		}
	}
}