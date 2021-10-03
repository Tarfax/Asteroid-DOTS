//Author: Mikael Cedergren


#include "Draw.h"

void InitRenderer() {
	SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
	SDL_RenderClear(engine.renderer);
}

void DrawRenderer() {
	SDL_RenderPresent(engine.renderer);
}

SDL_Texture* LoadTextureFromId(char* filename) {
	SDL_Texture* texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(engine.renderer, filename);
	return texture;
}

Sprite LoadTextureFromFont(char* filename, char* inText, Color color, int size) {
	font = TTF_OpenFont(filename, size);

	SDL_Color sdlColor;
	sdlColor.r = color.r;
	sdlColor.g = color.g;
	sdlColor.b = color.b;
	sdlColor.a = color.a;

	SDL_Surface* surface = TTF_RenderText_Solid(font, inText, sdlColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(engine.renderer, surface);

	Sprite sprite;

	sprite.texture = texture;
	sprite.width = surface->w;
	sprite.height = surface->h;

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = sprite.width;
	rect.h = sprite.height;

	sprite.rect = rect;
	sprite.color = color;

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	return sprite;
}

void DrawTexture(Sprite sprite, Vector2 position) {
	SDL_Rect destination;
	destination.x = position.x;
	destination.y = position.y;
	destination.w = sprite.width;
	destination.h = sprite.height;

	SDL_RenderCopyEx(engine.renderer, sprite.texture, &sprite.rect, &destination, sprite.rotation, NULL, SDL_FLIP_NONE);
}

void DrawHUD(Sprite sprite, Vector2 position) {
	SDL_Rect destination;
	destination.x = position.x;
	destination.y = position.y;
	destination.w = sprite.width;
	destination.h = sprite.height;

	//Can't get SDL_RenderCopyEx to work with fonts.
	SDL_RenderCopy(engine.renderer, sprite.texture, &sprite.rect, &destination);
}