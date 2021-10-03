//Author: Mikael Cedergren

#pragma once
#include <Common.h>

static void LoadSound(char* filename, int arrayPosition);
void LoadMusic(char* filename);

Mix_Chunk* sounds[SOUND_MAX];

Mix_Music* music;