//Author: Mikael Cedergren

#include "Physics.h"

int CollisionAABB(SDL_Rect t1, SDL_Rect t2) {
	return (MAX(t1.x, t2.x) < MIN(t1.x + t1.w, t2.x + t2.w)) && (MAX(t1.y, t2.y) < MIN(t1.y + t1.h, t2.y + t2.h));
}