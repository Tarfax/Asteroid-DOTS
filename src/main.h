//Author: Mikael Cedergren

#include "Common.h"

extern void CleanUp();
extern void InitSDL();
extern void InitSounds();
extern void InitGame();
extern void InitRenderer();
extern void DrawRenderer();
extern void HandleInput();
extern void Update();
extern void Draw();

Engine engine;
Input input;