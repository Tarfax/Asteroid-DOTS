//Author: Mikael Cedergren

#pragma once
#include <Common.h>

static void InitPlayer();
static void InitStarfield();
static void InitTextElements();
static void InitNextWave();

static void Update();
static void UpdateBackground();
static void UpdatePlayer();
static void UpdateBullets();
static void UpdateEnemies();
static void UpdateStarfield();
static void UpdateExplosions();
static void UpdateDebris();
static void UpdateScorePads();
static void UpdateCountdownWaveCounter();

static void Draw();
static void DrawBackground();
static void DrawStarfield();
static void DrawPlayer();
static void DrawBullets();
static void DrawEnemies();
static void DrawDebris();
static void DrawExplosions();
static void DrawText();
static void DrawScorePads();

static void Fire();
static void FireEnemyBullet(Entity* entity);
static void SpawnEnemies();
static void SpawnScorePads(Vector2 position);
static void CreateExplosion(Vector2 position, int count);
static void CalculateDirection(Vector2 a, Vector2 b, float* deltaX, float* deltaY);
static void ResetGame();

extern SDL_Texture* LoadTextureFromId(char* filename);
extern Sprite GetTextSprite(Color color, int size, char* inText, ...);
extern void DeleteFontTexture(SDL_Texture* texture);
extern void DrawTexture(Sprite sprite, Vector2 position);
extern void DrawHUD(Sprite sprite, Vector2 position);
extern int CollisionAABB(SDL_Rect t1, SDL_Rect t2);
extern void PlayMusic(int loop);
extern void PlaySound(int id, int channel);

GameInstance gameInstance; 

extern Engine engine;
extern Input input;

Entity* player;
SDL_Texture* playerBulletTexture; 
SDL_Texture* enemyBulletTexture; 
SDL_Texture* playerTexture;
SDL_Texture* enemyTexture;
SDL_Texture* scorePadTexture;
SDL_Texture* background;
SDL_Texture* explosionTexture;

int enemySpawnTimer;
int gameResetTimer;
int playerDeaths;
int highScore;

int secondsToNextWave;
int removeWaveSecondsTimer;
int isWaveCountDownVisible;

int isInWave;
int spawnedEnemies;
int spawnedEnemiesInternal;
int spawnedEnemiesCap;
int hasLastEnemyInWaveSpawned;

int backgroundX;

Star stars[MAX_STARS];
Text textElements[TEXT_ELEMENTS];