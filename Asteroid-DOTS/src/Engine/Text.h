//Author: Mikael Cedergren

#pragma once
#include <Common.h>

extern Sprite LoadTextureFromFont(char* filename, char* inText, Color color, int size);
extern void DrawTexture(Sprite sprite, Vector2 position);

