//Author: Mikael Cedergren

#include "Text.h"

Sprite GetTextSprite(Color color, int size, char* inText, ...) {
	char trimmedText[MAX_TEXT_LENGTH];

	va_list args;
	va_start(args, inText);
	memset(&trimmedText, '\0', sizeof(trimmedText));
	vsprintf_s(trimmedText, MAX_TEXT_LENGTH, inText, args);
	va_end(args);

	Sprite sprite;
	sprite = LoadTextureFromFont("res/fonts/novamono.ttf", trimmedText, color, size);
	return sprite;
}

void DeleteFontTexture(SDL_Texture* texture) {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}