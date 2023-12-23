#include "AABB.h"

bool CheckCollision(SDL_FRect rectA, SDL_FRect rectB)
{
	bool collisionX = rectA.x + rectA.w >= rectB.x && rectB.x + rectB.w >= rectA.x;
	bool collisionY = rectA.y + rectA.h >= rectB.y && rectB.y + rectB.h >= rectA.y;
	return collisionX && collisionY;
}
