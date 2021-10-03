//Author: Mikael Cedergren

#include "GameInstance.h"

void InitGame() {
	engine.delegate.update = Update;
	engine.delegate.draw = Draw;

	playerTexture = LoadTextureFromId("res/graphics/player.png");
	playerBulletTexture = LoadTextureFromId("res/graphics/player_bullet.png");
	enemyTexture = LoadTextureFromId("res/graphics/enemy.png");
	enemyBulletTexture = LoadTextureFromId("res/graphics/enemy_bullet.png");
	scorePadTexture = LoadTextureFromId("res/graphics/score_pad.png");
	background = LoadTextureFromId("res/graphics/background_stars.png");
	explosionTexture = LoadTextureFromId("res/graphics/explosion01.png");

	PlayMusic(0);

	ResetGame();
}

static void ResetGame() {
	Entity* entity;
	Explosion* explosion;
	Debris* debris;
	Text* text;

	while (gameInstance.entity_IndexFirst.next != NULL) {
		entity = gameInstance.entity_IndexFirst.next;
		gameInstance.entity_IndexFirst.next = entity->next;
		free(entity);
	}

	while (gameInstance.bullet_IndexFirst.next != NULL) {
		entity = gameInstance.bullet_IndexFirst.next;
		gameInstance.bullet_IndexFirst.next = entity->next;
		free(entity);
	}

	while (gameInstance.scorePads_IndexFirst.next != NULL) {
		entity = gameInstance.scorePads_IndexFirst.next;
		gameInstance.scorePads_IndexFirst.next = entity->next;
		free(entity);
	}

	while (gameInstance.explosion_IndexFirst.next != NULL) {
		explosion = gameInstance.explosion_IndexFirst.next;
		gameInstance.explosion_IndexFirst.next = explosion->next;
		free(explosion);
	}

	while (gameInstance.debris_IndexFirst.next != NULL) {
		debris = gameInstance.debris_IndexFirst.next;
		gameInstance.debris_IndexFirst.next = debris->next;
		free(debris);
	}

	for (int i = 0; i < TEXT_ELEMENTS; i++) {
		DeleteFontTexture(textElements[i].sprite.texture);
	}

	memset(&gameInstance, 0, sizeof(GameInstance));
	gameInstance.entity_IndexLast = &gameInstance.entity_IndexFirst;
	gameInstance.bullet_IndexLast = &gameInstance.bullet_IndexFirst;
	gameInstance.explosion_IndexLast = &gameInstance.explosion_IndexFirst;
	gameInstance.debris_IndexLast = &gameInstance.debris_IndexFirst;
	gameInstance.scorePads_IndexLast = &gameInstance.scorePads_IndexFirst;

	InitPlayer();
	InitStarfield();
	InitTextElements();
	enemySpawnTimer = 0;
	gameResetTimer = FPS * 2;

	PlaySound(Sound_GameStart, ChannelAny);
}

static void InitPlayer() {
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));

	gameInstance.entity_IndexLast->next = player;
	gameInstance.entity_IndexLast = player;

	player->position.x = 100;
	player->position.y = 100;
	player->sprite.rotation = 90;
	player->reload = 8;
	player->side = SIDE_PLAYER;
	player->health = 1;

	player->sprite.texture = playerTexture;
	SDL_QueryTexture(player->sprite.texture, NULL, NULL, &player->sprite.width, &player->sprite.height);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = player->sprite.width;
	rect.h = player->sprite.height;

	player->sprite.rect = rect;
}

static void InitStarfield() {
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

static void InitTextElements() {
	Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//Some magic numbers to place them on the screen

	Vector2 position;
	position.x = 8;
	position.y = 5;

	textElements[Text_Score].size = 69;
	textElements[Text_Score].sprite = GetTextSprite(color, textElements[Text_Score].size, "Score: %03d", 0);
	textElements[Text_Score].position = position;

	textElements[Text_PlayerDeaths].size = 42;
	textElements[Text_PlayerDeaths].sprite = GetTextSprite(color, textElements[Text_PlayerDeaths].size, "Player Deaths: %i", playerDeaths);
	position.x += textElements[Text_Score].sprite.width + 45;
	textElements[Text_PlayerDeaths].position = position;

	textElements[Text_PlayerShots].size = 42;
	textElements[Text_PlayerShots].sprite = GetTextSprite(color, textElements[Text_PlayerShots].size, "Player Shots: %i", 0);
	position.y += textElements[Text_PlayerDeaths].sprite.height + 8;
	textElements[Text_PlayerShots].position = position;

	textElements[Text_EnemyDeaths].size = 42;
	textElements[Text_EnemyDeaths].sprite = GetTextSprite(color, textElements[Text_EnemyDeaths].size, "Enemy Deaths: %i", 0);
	position.x += textElements[Text_PlayerDeaths].sprite.width + 20;
	position.y -= textElements[Text_PlayerDeaths].sprite.height + 8;
	textElements[Text_EnemyDeaths].position = position;

	textElements[Text_HighScore].size = 69;
	textElements[Text_HighScore].sprite = GetTextSprite(color, textElements[Text_HighScore].size, "HighScore: %03d", highScore);
	position.x = 980;
	position.y = 5;
	textElements[Text_HighScore].position = position;
}

static void Update() {
	UpdateBackground();
	UpdateStarfield();
	UpdatePlayer();
	UpdateEnemies();
	UpdateBullets();
	UpdateExplosions();
	UpdateDebris();
	UpdateScorePads();

	SpawnEnemies();

	if (player == NULL && --gameResetTimer <= 0) {
		ResetGame();
	}

}

static void UpdateBackground() {
	if (--backgroundX < -SCREEN_WIDTH) {
		backgroundX = 0;
	}
}

static void UpdatePlayer() {
	if (player != NULL) {

		player->velocity.x = 0;
		player->velocity.y = 0;

		if (player->reload > 0) {
			player->reload--;
		}

		if (input.keyboard[SDL_SCANCODE_W] == 1) {
			player->velocity.y = -PLAYER_SPEED;
		}

		if (input.keyboard[SDL_SCANCODE_S] == 1) {
			player->velocity.y = PLAYER_SPEED;
		}

		if (input.keyboard[SDL_SCANCODE_A] == 1) {
			player->velocity.x = -PLAYER_SPEED;
		}

		if (input.keyboard[SDL_SCANCODE_D] == 1) {
			player->velocity.x = PLAYER_SPEED;
		}

		if (input.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0) {
			Fire();
		}

		player->position.x += player->velocity.x;
		player->position.y += player->velocity.y;
	}
}

static void UpdateBullets() {
	Entity* bullet;
	Entity* previousBullet;

	//First bullet to iterate
	previousBullet = &gameInstance.bullet_IndexFirst;

	//Linked list iterator
	for (bullet = gameInstance.bullet_IndexFirst.next; bullet != NULL; bullet = bullet->next) {
		//Update position
		bullet->position.x += bullet->velocity.x;
		bullet->position.y += bullet->velocity.y;

		//Kill the bullet if its outside the screen
		if (CheckIfBulletHitEnemy(bullet) > 0 || bullet->position.x < -bullet->sprite.width || bullet->position.y < -bullet->sprite.height || bullet->position.x > SCREEN_WIDTH || bullet->position.y > SCREEN_HEIGHT) {
			if (bullet == gameInstance.bullet_IndexLast) {
				gameInstance.bullet_IndexLast = previousBullet;
			}
			previousBullet->next = bullet->next;

			//Like delete in c++
			free(bullet);

			bullet = previousBullet;

		}

		previousBullet = bullet;
	}
}

static void UpdateEnemies() {
	Entity* enemy;
	Entity* previousEnemy;

	previousEnemy = &gameInstance.entity_IndexFirst;

	//Linked list iterator
	for (enemy = gameInstance.entity_IndexFirst.next; enemy != NULL; enemy = enemy->next) {

		enemy->position.x += enemy->velocity.x;
		enemy->position.y += enemy->velocity.y;

		if (enemy != player && player != NULL && --enemy->reload <= 0) {
			FireEnemyBullet(enemy);
		}

		if (enemy != player && enemy->position.x < -enemy->sprite.width) {
			enemy->health = 0;
		}

		if (enemy->health == 0) {
			if (enemy == player) {
				player = NULL;

				DeleteFontTexture(textElements[Text_PlayerDeaths].sprite.texture);
				textElements[Text_PlayerDeaths].sprite = GetTextSprite(textElements[Text_PlayerDeaths].sprite.color, textElements[Text_PlayerDeaths].size, "Player Deaths: %i", ++playerDeaths);
			}


			if (enemy == gameInstance.entity_IndexLast) {
				gameInstance.entity_IndexLast = previousEnemy;
			}

			previousEnemy->next = enemy->next;
			free(enemy);
			enemy = previousEnemy;

		}

		previousEnemy = enemy;
	}

}

static void UpdateStarfield() {
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x -= stars[i].speed;

		if (stars[i].x < 0) {
			stars[i].x = SCREEN_WIDTH + stars[i].x;
		}
	}
}

static void UpdateExplosions() {
	Explosion* explosion;
	Explosion* previousExplosion;

	previousExplosion = &gameInstance.explosion_IndexFirst;

	for (explosion = gameInstance.explosion_IndexFirst.next; explosion != NULL; explosion = explosion->next) {
		explosion->position.x += explosion->velocity.x;
		explosion->position.y += explosion->velocity.y;

		if (--explosion->sprite.color.a <= 0) {
			if (explosion == gameInstance.explosion_IndexLast) {
				gameInstance.explosion_IndexLast = previousExplosion;
			}

			previousExplosion->next = explosion->next;
			free(explosion);
			explosion = previousExplosion;
		}

		previousExplosion = explosion;
	}
}

static void UpdateDebris() {
	Debris* debris;
	Debris* previousDebris;

	previousDebris = &gameInstance.debris_IndexFirst;

	for (debris = gameInstance.debris_IndexFirst.next; debris != NULL; debris = debris->next) {

		debris->position.x += debris->velocity.x;
		debris->position.y += debris->velocity.y;

		debris->velocity.y += 0.5;

		if (--debris->life <= 0) {
			if (debris == gameInstance.debris_IndexLast) {
				gameInstance.debris_IndexLast = previousDebris;
			}

			previousDebris->next = debris->next;
			free(debris);
			debris = previousDebris;
		}
		previousDebris = debris;
	}
}

static void UpdateScorePads() {
	Entity* scorePad;
	Entity* previousScorePad;

	previousScorePad = &gameInstance.scorePads_IndexFirst;

	for (scorePad = gameInstance.scorePads_IndexFirst.next; scorePad != NULL; scorePad = scorePad->next) {
		//printf("Update Score Pads\n");

		if (scorePad->position.x < 0) {
			scorePad->position.x = 0;
			scorePad->velocity.x = -scorePad->velocity.x;
		}

		if (scorePad->position.x + scorePad->sprite.width > SCREEN_WIDTH) {
			scorePad->position.x = SCREEN_WIDTH - scorePad->sprite.width;
			scorePad->velocity.x = -scorePad->velocity.x;
		}

		if (scorePad->position.y < 0) {
			scorePad->position.y = 0;
			scorePad->velocity.y = -scorePad->velocity.y;
		}

		if (scorePad->position.y + scorePad->sprite.height > SCREEN_HEIGHT) {
			scorePad->position.y = SCREEN_HEIGHT - scorePad->sprite.height;
			scorePad->velocity.y = -scorePad->velocity.y;
		}

		scorePad->position.x += scorePad->velocity.x;
		scorePad->position.y += scorePad->velocity.y;

		if (player != NULL) {
			SDL_Rect scorePadRect;
			SDL_Rect playerRect;

			scorePadRect.x = scorePad->position.x;
			scorePadRect.y = scorePad->position.y;
			scorePadRect.w = scorePad->sprite.width;
			scorePadRect.h = scorePad->sprite.height;

			playerRect.x = player->position.x;
			playerRect.y = player->position.y;
			playerRect.w = player->sprite.width;
			playerRect.h = player->sprite.height;

			if (CollisionAABB(scorePadRect, playerRect) == 1) {
				scorePad->health = 0;
				PlaySound(Sound_ScorePad, ChannelScorePad);

				gameInstance.score += 25;
				highScore = MAX(gameInstance.score, highScore);
				DeleteFontTexture(textElements[Text_Score].sprite.texture);
				textElements[Text_Score].sprite = GetTextSprite(textElements[Text_Score].sprite.color, textElements[Text_Score].size, "Score: %03d", gameInstance.score);

				DeleteFontTexture(textElements[Text_HighScore].sprite.texture);
				textElements[Text_HighScore].sprite = GetTextSprite(textElements[Text_HighScore].sprite.color, textElements[Text_HighScore].size, "High Score: %03d", highScore);
			}

			if (--scorePad->health <= 0) {
				if (scorePad == gameInstance.scorePads_IndexLast) {
					gameInstance.scorePads_IndexLast = previousScorePad;
				}

				previousScorePad->next = scorePad->next;
				free(scorePad);
				scorePad = previousScorePad;
			}

			previousScorePad = scorePad;
		}

	}
}

static void Draw() {
	DrawBackground();
	DrawStarfield();
	DrawScorePads();
	DrawPlayer();
	DrawEnemies();
	DrawDebris();
	DrawExplosions();
	DrawBullets();
	DrawText();
}

static void DrawBackground() {
	SDL_Rect destination;

	for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH) {
		destination.x = x;
		destination.y = 0;
		destination.w = SCREEN_WIDTH;
		destination.h = SCREEN_HEIGHT;

		SDL_RenderCopy(engine.renderer, background, NULL, &destination);
	}
}

static void DrawStarfield() {
	for (int i = 0; i < MAX_STARS; i++) {
		int c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(engine.renderer, c, c, c, 255);
		SDL_RenderDrawLine(engine.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}

static void DrawPlayer() {
	if (player != NULL) {
		if (player->position.x < 0) {
			player->position.x = 0;
		}
		if (player->position.y < 0) {
			player->position.y = 0;
		}
		if (player->position.x > SCREEN_WIDTH - player->sprite.width) {
			player->position.x = SCREEN_WIDTH - player->sprite.width;
		}
		if (player->position.y > SCREEN_HEIGHT - player->sprite.height) {
			player->position.y = SCREEN_HEIGHT - player->sprite.height;
		}

		DrawTexture(player->sprite, player->position);
	}

}

static void DrawEnemies() {
	Entity* enemy;

	for (enemy = gameInstance.entity_IndexFirst.next; enemy != NULL; enemy = enemy->next) {
		DrawTexture(enemy->sprite, enemy->position);
	}
}

static void DrawBullets() {
	Entity* bullet;

	for (bullet = gameInstance.bullet_IndexFirst.next; bullet != NULL; bullet = bullet->next) {
		DrawTexture(bullet->sprite, bullet->position);
	}
}

static void DrawDebris() {
	Debris* debris;

	for (debris = gameInstance.debris_IndexFirst.next; debris != NULL; debris = debris->next) {
		DrawTexture(debris->sprite, debris->position);
	}
}

static void DrawExplosions() {
	Explosion* explosion;

	SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (explosion = gameInstance.explosion_IndexFirst.next; explosion != NULL; explosion = explosion->next) {
		SDL_SetTextureColorMod(explosionTexture, explosion->sprite.color.r, explosion->sprite.color.g, explosion->sprite.color.b);
		SDL_SetTextureAlphaMod(explosionTexture, explosion->sprite.color.a);
		SDL_SetTextureAlphaMod(explosionTexture, 255);

		DrawTexture(explosion->sprite, explosion->position);
	}

	SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_NONE);

}

static void DrawText() {
	for (int i = 0; i < TEXT_ELEMENTS; i++) {
		DrawHUD(textElements[i].sprite, textElements[i].position);
	}
}

void DrawScorePads() {
	Entity* scorePad;

	for (scorePad = gameInstance.scorePads_IndexFirst.next; scorePad != NULL; scorePad = scorePad->next) {
		DrawTexture(scorePad->sprite, scorePad->position);
	}
}


static void Fire() {
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));

	//If more bullets are in the level, set this bullet to be the last bullet in the "array"
	gameInstance.bullet_IndexLast->next = bullet;
	gameInstance.bullet_IndexLast = bullet;

	//Init the bullets variables
	bullet->position.x = player->position.x;
	bullet->position.y = player->position.y;
	bullet->sprite.rotation = 90;
	bullet->velocity.x = PLAYER_BULLET_SPEED;
	//bullet->deltaY = 0;
	bullet->health = 1;
	bullet->side = SIDE_PLAYER;

	//Give the bullet the texture
	bullet->sprite.texture = playerBulletTexture;
	SDL_QueryTexture(bullet->sprite.texture, NULL, NULL, &bullet->sprite.width, &bullet->sprite.height);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = bullet->sprite.width;
	rect.h = bullet->sprite.height;

	bullet->sprite.rect = rect;

	//Give the bullet its spawn y position
	bullet->position.x += (player->sprite.width / 2) - (bullet->sprite.width / 2);
	bullet->position.y += (player->sprite.height / 2) - (bullet->sprite.height / 2);


	//Give the player 8 frames of reload time
	player->reload = 8;

	PlaySound(Sound_PlayerFire, ChannelPlayer);

	gameInstance.playerShots++;
	gameInstance.score--;
	DeleteFontTexture(textElements[Text_PlayerShots].sprite.texture);
	textElements[Text_PlayerShots].sprite = GetTextSprite(textElements[Text_PlayerShots].sprite.color, textElements[Text_PlayerShots].size, "Player Shots: %i", gameInstance.playerShots);
	DeleteFontTexture(textElements[Text_Score].sprite.texture);
	textElements[Text_Score].sprite = GetTextSprite(textElements[Text_Score].sprite.color, textElements[Text_Score].size, "Score: %03d", gameInstance.score);
}

static void FireEnemyBullet(Entity* entity) {
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));

	gameInstance.bullet_IndexLast->next = bullet;
	gameInstance.bullet_IndexLast = bullet;

	bullet->health = 1;
	bullet->sprite.texture = enemyBulletTexture;
	SDL_QueryTexture(bullet->sprite.texture, NULL, NULL, &bullet->sprite.width, &bullet->sprite.height);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = bullet->sprite.width;
	rect.h = bullet->sprite.height;

	bullet->sprite.rect = rect;

	bullet->position.x = entity->position.x;
	bullet->position.y = entity->position.y;
	bullet->position.x += (entity->sprite.width / 2) - (bullet->sprite.width / 2);
	bullet->position.y += (entity->sprite.height / 2) - (bullet->sprite.height / 2);


	Vector2 playerPosition;
	playerPosition.x = player->position.x + (player->sprite.width / 2);
	playerPosition.y = player->position.y + (player->sprite.height / 2);
	Vector2 entityPosition = entity->position;

	CalculateDirection(playerPosition, entityPosition, &bullet->velocity.x, &bullet->velocity.y);
	bullet->sprite.rotation = 90;

	bullet->velocity.x *= ENEMY_BULLET_SPEED;
	bullet->velocity.y *= ENEMY_BULLET_SPEED;

	bullet->side = SIDE_ENEMY;

	entity->reload = (rand() % FPS * 4);

	PlaySound(Sound_EnemyFire, ChannelEnemyFire);

}


static void SpawnEnemies() {
	Entity* enemy;

	if (--enemySpawnTimer <= 0) {
		enemy = malloc(sizeof(Entity));
		memset(enemy, 0, sizeof(Entity));

		gameInstance.entity_IndexLast->next = enemy;
		gameInstance.entity_IndexLast = enemy;

		enemy->position.x = SCREEN_WIDTH;
		enemy->position.y = rand() % SCREEN_HEIGHT;
		enemy->sprite.texture = enemyTexture;
		enemy->sprite.rotation = 270;
		enemy->side = SIDE_ENEMY;
		enemy->health = 2;

		SDL_QueryTexture(enemy->sprite.texture, NULL, NULL, &enemy->sprite.width, &enemy->sprite.height);

		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = enemy->sprite.width;
		rect.h = enemy->sprite.height;

		enemy->sprite.rect = rect;

		enemy->velocity.x = -(2 + (rand() % 4));

		enemySpawnTimer = 30 + (rand() % 60);
		enemy->reload = FPS * (1 + (rand() % 3));
	}
}

static void SpawnScorePads(Vector2 position) {
	Entity* scorePad;

	scorePad = malloc(sizeof(Entity));
	memset(scorePad, 0, sizeof(Entity));
	gameInstance.scorePads_IndexLast->next = scorePad;
	gameInstance.scorePads_IndexLast = scorePad;

	scorePad->health = FPS * 10;

	scorePad->sprite.texture = scorePadTexture;
	SDL_QueryTexture(scorePad->sprite.texture, NULL, NULL, &scorePad->sprite.width, &scorePad->sprite.height);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = scorePad->sprite.width;
	rect.h = scorePad->sprite.height;

	scorePad->sprite.rect = rect;

	scorePad->position.x = position.x;
	scorePad->position.y = position.y;
	scorePad->position.x -= scorePad->sprite.width / 2;
	scorePad->position.y -= scorePad->sprite.height / 2;

	scorePad->velocity.x = -(rand() % 5);
	scorePad->velocity.y = (rand() % 5) - (rand() % 5);
}

static void CreateExplosion(Vector2 position, int count) {
	Explosion* explosion;

	for (int i = 0; i < count; i++) {

		explosion = malloc(sizeof(Explosion));
		memset(explosion, 0, sizeof(Explosion));
		gameInstance.explosion_IndexLast->next = explosion;
		gameInstance.explosion_IndexLast = explosion;

		explosion->position.x = position.x + (rand() % 32) - (rand() % 32);
		explosion->position.y = position.y + (rand() % 32) - (rand() % 32);

		explosion->velocity.x = (rand() % 10) - (rand() % 10);
		explosion->velocity.y = (rand() % 10) - (rand() % 10);

		explosion->velocity.x /= 10;
		explosion->velocity.y /= 10;

		switch (rand() % 4) {
			case 0:
				explosion->sprite.color.r = 255;
				break;
			case 1:
				explosion->sprite.color.r = 255;
				explosion->sprite.color.g = 128;
				break;
			case 2:
				explosion->sprite.color.r = 255;
				explosion->sprite.color.g = 255;
				break;
			default:
				explosion->sprite.color.r = 255;
				explosion->sprite.color.g = 255;
				explosion->sprite.color.b = 255;
				break;
		}

		explosion->sprite.color.a = rand() % FPS * 3;

		explosion->sprite.texture = explosionTexture;
		SDL_QueryTexture(explosion->sprite.texture, NULL, NULL, &explosion->sprite.width, &explosion->sprite.height);

		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = explosion->sprite.width;
		rect.h = explosion->sprite.height;

		explosion->sprite.rect = rect;
	}
}

static void CreateDebris(Entity* entity) {
	Debris* debris;
	SDL_Rect rect;

	rect.w = entity->sprite.width / 4;
	rect.h = entity->sprite.height / 4;

	for (rect.y = 0; rect.y <= rect.h; rect.y += rect.h) {
		for (rect.x = 0; rect.x <= rect.w; rect.x += rect.w) {
			debris = malloc(sizeof(Debris));
			memset(debris, 0, sizeof(Debris));
			gameInstance.debris_IndexLast->next = debris;
			gameInstance.debris_IndexLast = debris;

			debris->position.x = entity->position.x + entity->sprite.width / 2;
			debris->position.y = entity->position.y + entity->sprite.height / 2;

			debris->velocity.x = (rand() % 5) - (rand() % 5);
			debris->velocity.y = -(5 + rand() % 12);

			debris->life = FPS * 2;
			debris->sprite.texture = entity->sprite.texture;

			debris->sprite.width = rect.w;
			debris->sprite.height = rect.h;

			debris->sprite.rect.x = rect.x;
			debris->sprite.rect.y = rect.y;
			debris->sprite.rect.w = rect.w;
			debris->sprite.rect.h = rect.h;
		}
	}
}

static int CheckIfBulletHitEnemy(Entity* bullet) {
	Entity* enemy;

	for (enemy = gameInstance.entity_IndexFirst.next; enemy != NULL; enemy = enemy->next) {
		SDL_Rect t1;
		t1.x = bullet->position.x;
		t1.y = bullet->position.y;
		t1.w = bullet->sprite.width;
		t1.h = bullet->sprite.height;

		SDL_Rect t2;
		t2.x = enemy->position.x;
		t2.y = enemy->position.y;
		t2.w = enemy->sprite.width;
		t2.h = enemy->sprite.height;


		if (enemy->side != bullet->side && CollisionAABB(t1, t2) == 1) {
			bullet->health = 0;
			enemy->health = 0;
			if (enemy == player) {
				PlaySound(Sound_PlayerDie, ChannelPlayer);
			}
			else {
				Vector2 scorePadPosition;
				scorePadPosition.x = enemy->position.x + enemy->sprite.width / 2;
				scorePadPosition.y = enemy->position.y + enemy->sprite.height / 2;
				SpawnScorePads(scorePadPosition);

				PlaySound(Sound_EnemyDie, ChannelEnemyDie);

				DeleteFontTexture(textElements[Text_EnemyDeaths].sprite.texture);
				textElements[Text_EnemyDeaths].sprite = GetTextSprite(textElements[Text_EnemyDeaths].sprite.color, textElements[Text_EnemyDeaths].size, "Enemy Deaths: %i", ++gameInstance.enemyDeaths);

				gameInstance.score += 10;
				DeleteFontTexture(textElements[Text_Score].sprite.texture);
				textElements[Text_Score].sprite = GetTextSprite(textElements[Text_Score].sprite.color, textElements[Text_Score].size, "Score: %03d", gameInstance.score);

				highScore = MAX(gameInstance.score, highScore);
				DeleteFontTexture(textElements[Text_HighScore].sprite.texture);
				textElements[Text_HighScore].sprite = GetTextSprite(textElements[Text_HighScore].sprite.color, textElements[Text_HighScore].size, "High Score: %03d", highScore);
			}

			CreateExplosion(enemy->position, 32);
			CreateDebris(enemy);
			return 1;
		}
	}

	return 0;
}


static void CalculateDirection(Vector2 a, Vector2 b, float* deltaX, float* deltaY) {
	int steps = MAX(abs(a.x - b.x), abs(a.y - b.y));

	if (steps == 0) {
		*deltaX = 0;
		*deltaY = 0;
		return;
	}

	*deltaX = (a.x - b.x);
	*deltaX /= steps;

	*deltaY = (a.y - b.y);
	*deltaY /= steps;
}
