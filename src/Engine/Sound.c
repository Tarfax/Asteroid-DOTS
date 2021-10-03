//Author: Mikael Cedergren

#include "Sound.h"

void InitSounds() {
	memset(sounds, 0, sizeof(Mix_Chunk*) * SOUND_MAX);

	music = NULL;

	LoadSound("res/sound/player_fire.wav", Sound_PlayerFire);
	LoadSound("res/sound/enemy_fire.wav", Sound_EnemyFire);
	LoadSound("res/sound/player_die.wav", Sound_PlayerDie);
	LoadSound("res/sound/enemy_die.wav", Sound_EnemyDie);
	LoadSound("res/sound/game_start.wav", Sound_GameStart);
	LoadSound("res/sound/score_pad.wav", Sound_ScorePad);

	LoadMusic("res/sound/music.ogg");

	printf("Init Sound SUCCESS\n");
}

static void LoadSound(char* filename, int arrayPosition) {
	sounds[arrayPosition] = Mix_LoadWAV(filename);
	Mix_VolumeChunk(sounds[arrayPosition], 35);
}

void LoadMusic(char* filename) {
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	music = Mix_LoadMUS(filename);
	if (Mix_PlayMusic(music, -1) == -1) {
		printf("Error loading playing music %s\n", SDL_GetError());
	}
	Mix_VolumeMusic(100);
	printf("Load Music\n");
}

void PlayMusic(int loop) {
	//Mix_PlayMusic(music, (loop) ? -1 : 0);
	printf("Play music\n");
}

void PlaySound(int id, int channel) {
	Mix_PlayChannel(channel, sounds[id], 0);
}