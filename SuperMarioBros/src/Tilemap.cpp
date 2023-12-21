#include "Tilemap.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>


static std::unordered_map<std::string, std::string> tilemap;

static std::vector<Tile> tiles;

static uint32_t xTilemapOffset = 0;
static uint32_t yTilemapOffset = 0;

void LoadTilemap(const std::string& name, const std::string& path, Tile tile, const TilemapCallbackfn& fn)
{
	if (tilemap.find(name) != tilemap.end())
	{
		std::cout << "Key already exist \n";
		return;
	}

	std::ifstream file;

	file.open(path, std::ios::in | std::ios::binary);
	
	std::string tilemapLine;
	while (std::getline(file, tilemapLine))
	{
		// tilemap[name].append(tilemapLine);
		for (size_t i = 0; i < tilemapLine.size(); i++)
		{
			if (tilemapLine[i] == ' ')
			{
				xTilemapOffset++;
			}

			if (tilemapLine[i] == '\r' || tilemapLine[i] == '\n')
			{
				yTilemapOffset++;
				xTilemapOffset = 0;
				continue;
			}
s
			if (fn(tilemapLine[i]) != -1)
			{
				xTilemapOffset++;
				Tile t = { 0 };
				t.x = tile.Width * xTilemapOffset;
				t.y = tile.Height * yTilemapOffset;
				t.Width = tile.Width;
				t.Height = tile.Height;
				t.Type = fn(tilemapLine[i]);
				tiles.emplace_back(t);
			}
		}

	}

	file.close();

	xTilemapOffset = 0;
	yTilemapOffset = 0;
}

void RenderTilemap(SDL_Rect camera, Texture texture, std::vector<SDL_Rect>& tileset)
{
	for (const auto& tile : tiles)
	{
		RenderTextureClip(tile.x - camera.x, tile.y - camera.y, texture, &tileset[tile.Type]);
	}
}

void SetTilemap(const std::string& name)
{
	if (tilemap.find(name) != tilemap.end())
	{
		std::cout << "Key exist \n";

	}
	else
	{
		std::cout << "Key doesn't exist \n";
		return;
	}
}

