#pragma once

#include <SDL.h>

#include <cinttypes>
#include <string>
#include <vector>
#include <functional>

#include "Texture.h"

struct Tile
{
	uint32_t x;
	uint32_t y;
	uint32_t Width;
	uint32_t Height;
	int Type;
};

using TilemapCallbackfn = std::function<int(char)>;

void LoadTilemap(const std::string& name, const std::string& path, Tile tile, const TilemapCallbackfn& fn);
void RenderTilemap(SDL_Rect camera, Texture texture, std::vector<SDL_Rect>& tileset);
void SetTilemap(const std::string& name);
