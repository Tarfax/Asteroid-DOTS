//Author: Mikael Cedergren

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define FPS 60
#define MAX_AUDIO_CHANNELS 8
#define SOUND_MAX 6

#define MAX_KEYBOARD_KEYS 350

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

#define PLAYER_SPEED 6
#define PLAYER_BULLET_SPEED 16
#define ENEMY_BULLET_SPEED 8

#define SIDE_PLAYER 0
#define SIDE_ENEMY 1

#define MAX_STARS 500

#define MAX_TEXT_LENGTH 1024
#define TEXT_ELEMENTS 6


enum {
	ChannelAny = -1,
	ChannelPlayer = 1,
	ChannelEnemyDie = 2,
	ChannelEnemyFire = 3,
	ChannelScorePad = 4,
};

enum {
	Sound_PlayerFire = 0,
	Sound_PlayerDie = 1,
	Sound_EnemyFire = 2,
	Sound_EnemyDie = 3,
	Sound_GameStart = 4,
	Sound_ScorePad = 5
};

enum {
	Text_PlayerShots = 0,
	Text_PlayerDeaths = 1,
	Text_EnemyDeaths = 2,
	Text_Score = 3,
	Text_HighScore = 4,
	Text_NextEnemyWave = 5

};