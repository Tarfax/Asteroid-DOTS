//Author: Mikael Cedergren

typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef struct Text Text;

typedef struct {
	void (*update)();
	void (*draw)();
} Delegate;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
} Engine;

typedef struct {
	int keyboard[MAX_KEYBOARD_KEYS];
} Input;

typedef struct {
	int r;
	int g;
	int b;
	int a;
} Color;

typedef struct {
	int width;
	int height;
	double rotation;
	SDL_Rect rect;
	Color color;
	SDL_Texture* texture;
} Sprite;

typedef struct {
	int x;
	int y;
} Vector2Int;

typedef struct {
	float x;
	float y;
} Vector2;

struct Text {
	Vector2 position;	
	int size;
	Sprite sprite;
};

struct Entity {
	Vector2 position;
	Vector2 velocity;

	int health;
	int reload;
	int side;

	Entity* next;

	Sprite sprite;
};

struct Explosion {
	Vector2 position;
	Vector2 velocity;
	Sprite sprite;
	Explosion* next;
};

struct Debris {
	Vector2 position;
	Vector2 velocity;
	Sprite sprite;
	int life;
	Debris* next;
};

typedef struct {
	Entity* entity_IndexLast;
	Entity* bullet_IndexLast;
	Entity* scorePads_IndexLast;
	Explosion* explosion_IndexLast;
	Debris* debris_IndexLast;
	Entity entity_IndexFirst;
	Entity bullet_IndexFirst;
	Entity scorePads_IndexFirst;
	Explosion explosion_IndexFirst;
	Debris debris_IndexFirst;

	int score;
	int playerShots;
	int enemyDeaths;
} GameInstance;


typedef struct {
	int x;
	int y;
	int speed;
} Star;