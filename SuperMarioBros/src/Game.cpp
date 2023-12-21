#include "Game.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

#include "Application.h"
#include "Texture.h"

enum class TileType
{
	BRICK_BASE			= 0,
	BRICK_SOLID_SHINE	= 1,
	BRICK_SOLID			= 2,
	BRICK_HARD_SOLID	= 3
};

struct Tile
{
	uint32_t x;
	uint32_t y;
	uint32_t Width;
	uint32_t Height;
	TileType Type;
};

// Tile
static SDL_Rect tileClips[4];
static std::fstream tilemapData;
static std::string tilemap;
static uint32_t xTilemapOffset = 0;
static uint32_t yTilemapOffset = 0;

// Camera
SDL_Rect camera;

// Animation Clips
static uint32_t SPRITE_SIZE = 16;
static SDL_Rect marioRunningClip[4];

// Animation frames
static const int MAX_FRAME = 3;
static int frame = 0;

struct Mario
{
	float x;
	float y;
};

static Mario mario;

void StartGame(GameState& state)
{
	if (!state.Instance)
	{
		state.Instance = std::make_unique<GameState>();
		std::cout << "Successully created a game instance\n";
	}

	state.Instance->sprites["supermario_atlas"] = LoadTexture("assets/images/SuperMarioBrosAtlas.png");
	tilemapData.open("assets/maps/level-1-1.txt", std::ios::in | std::ios::binary);

	if (!tilemapData.is_open())
	{
		std::cout << "Unable to open file\n";
		return;
	}

	std::string tilemapLine;
	while (std::getline(tilemapData, tilemapLine))
	{
		tilemap.append(tilemapLine);
	}

	tilemapData.close();

	// Tiles
	for (int i = 0; i < 4; i++)
	{
		const int TILE_SPRITE_OFFSET = (i + 14);
		tileClips[i].x = SPRITE_SIZE * TILE_SPRITE_OFFSET;
		tileClips[i].y = 0;
		tileClips[i].w = SPRITE_SIZE;
		tileClips[i].h = SPRITE_SIZE;
	}

	// Mario
	for (int i = 0; i < 4; i++)
	{
		const int MARIO_SPRITE_OFFSET = (i);
		marioRunningClip[i].x = SPRITE_SIZE * MARIO_SPRITE_OFFSET;
		marioRunningClip[i].y = 0;
		marioRunningClip[i].w = SPRITE_SIZE;
		marioRunningClip[i].h = SPRITE_SIZE;
	}
	
	// Mario should be placed right above the level
	// mario.y = 382 - SPRITE_SIZE;
	mario.y = 750 - SPRITE_SIZE;

	// Camera
	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);
	camera = { 0, 0, w, h };
}

void UpdateGame(GameState& state)
{
	SPRITE_SIZE = 16 * GetApplicationProps().Zoom;
	Texture atlas = state.Instance->sprites["supermario_atlas"];

	xTilemapOffset = 0;
	yTilemapOffset = 0;

	// Level
	for (size_t i = 0; i < tilemap.size(); i++)
	{
		if (tilemap[i] == ' ')
		{
			xTilemapOffset++;
			continue;
		}

		if (tilemap[i] == '\r' || tilemap[i] == '\n')
		{
			xTilemapOffset = 0;
			yTilemapOffset++;
			continue;
		}

		if (tilemap[i] == '#')
		{
			float x = SPRITE_SIZE * xTilemapOffset;
			float y = SPRITE_SIZE * yTilemapOffset;
			xTilemapOffset++;
			RenderTextureClip(x - camera.x, y - camera.y, atlas, &tileClips[(int)TileType::BRICK_BASE]);
		}
	}

	// Mario
	const Uint8* input = SDL_GetKeyboardState(nullptr);
	static bool isMoving;


	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);
	// SDL_GetRendererOutputSize(reinterpret_cast<SDL_Renderer*>(Renderer()), &w, &h);

	camera.x = mario.x - w / 2;
	camera.y = mario.y - h / 2;

	if (input[SDL_SCANCODE_A])
	{
		mario.x -= 1;
		isMoving = true;
	}
	else if (input[SDL_SCANCODE_D])
	{
		mario.x += 1;
		isMoving = true;
	}
	else
	{
		isMoving = false;
	}

	if (isMoving)
	{
		RenderTextureClip(mario.x - camera.x, mario.y - camera.y, atlas, &marioRunningClip[(frame + 1) / 3]);
		++frame;

		if (frame / 3 >= MAX_FRAME)
		{
			frame = 0;
		}
	}
	else
	{
		RenderTextureClip(mario.x - camera.x, mario.y - camera.y, atlas, &marioRunningClip[0]);
	}
}

void ShutdownGame(GameState& state)
{
	for (auto& [key, val] : state.Instance->sprites)
	{
		SDL_DestroyTexture(val.data);
	}
}