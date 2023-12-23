#pragma once

#include <SDL.h>

#include <cinttypes>
#include <string>
#include <vector>
#include <functional>

#include "Texture.h"

struct Tile
{
	float x;
	float y;
	float Width;
	float Height;
	int Type;
};

using TilemapCallbackfn = std::function<int(char)>;
using TilemapCollisionCallback = std::function<void(Tile)>;

void LoadTilemap(const std::string& name, const std::string& path, Tile tile, const TilemapCallbackfn& fn);
void RenderTilemap(SDL_FRect camera, Texture texture, std::vector<SDL_Rect>& tileset);
void SetTilemap(const std::string& name);
void CheckTilemapCollision(const SDL_FRect col, const TilemapCollisionCallback& fn);
