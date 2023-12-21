#include "Game.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>

#include "Application.h"
#include "Texture.h"
#include "Tilemap.h"

enum class TileType
{
	NONE				= -1,
	BRICK_BASE			=  0,
	BRICK_SOLID_SHINE	=  1,
	BRICK_SOLID			=  2,
	BRICK_HARD_SOLID	=  3
};

static const uint32_t SPRITE_SIZE = 16;
static SDL_Rect camera;

// Tile
static std::vector<SDL_Rect> tileClips;

static std::fstream tilemapData;
static std::string tilemap;

static uint32_t xTilemapOffset = 0;
static uint32_t yTilemapOffset = 0;

struct Mario
{
	float x;
	float y;
};

static const uint32_t SPEED = 200;
static Mario mario;

static std::vector<SDL_Rect> marioRunningClips;
static const int MAX_FRAME = 3;
static int frame = 0;

void StartGame(GameState& state)
{
	if (!state.Instance)
	{
		state.Instance = std::make_unique<GameState>();
		std::cout << "Successully created a game instance\n";
	}

	state.Instance->sprites["supermario_atlas"] = LoadTexture("assets/images/SuperMarioBrosAtlas.png");

	// Tiles
	for (int i = 0; i < 4; i++)
	{
		const int TILE_SPRITE_OFFSET = (i + 14);
		SDL_Rect tileClip = { 0 };
		tileClip.x = SPRITE_SIZE * TILE_SPRITE_OFFSET;
		tileClip.y = 0;
		tileClip.w = SPRITE_SIZE;
		tileClip.h = SPRITE_SIZE;
		tileClips.push_back(tileClip);
	}

	// Mario
	for (int i = 0; i < 4; i++)
	{
		const int MARIO_SPRITE_OFFSET = (i);
		SDL_Rect marioClip = { 0 };
		marioClip.x = SPRITE_SIZE * MARIO_SPRITE_OFFSET;
		marioClip.y = 0;
		marioClip.w = SPRITE_SIZE;
		marioClip.h = SPRITE_SIZE;
		marioRunningClips.push_back(marioClip);
	}
	
	// Mario should be placed right above the level
	// mario.y = 382 - SPRITE_SIZE;
	mario.y = 750 - SPRITE_SIZE;

	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);
	camera = { 0, 0, w, h };

	// Should resize the sprite after loading the clip textures
	uint32_t RESIZE_SPRITE_SIZE = 16 * GetApplicationProps().Zoom;

	Tile tileProps = {
		0, 0, RESIZE_SPRITE_SIZE, RESIZE_SPRITE_SIZE
	};

	LoadTilemap("Level-1-1", "assets/maps/level-1-1.txt", tileProps, [](char tile) -> int {
		TileType type = TileType::NONE;

		switch (tile)
		{
		case '#':
			type = TileType::BRICK_BASE;
			break;
		}

		return (int)type;
	});
}

void UpdateGame(GameState& state)
{
	Texture atlas = state.Instance->sprites["supermario_atlas"];

	xTilemapOffset = 0;
	yTilemapOffset = 0;

	// Level
	RenderTilemap(camera, atlas, tileClips);

	// Mario
	const Uint8* input = SDL_GetKeyboardState(nullptr);
	static bool isMoving;

	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);

	camera.x = mario.x - w / 2;
	camera.y = mario.y - h / 2;

	double velocityX = SPEED * state.DeltaTime;
	if (input[SDL_SCANCODE_A])
	{
		mario.x -= velocityX;
		isMoving = true;
	}
	else if (input[SDL_SCANCODE_D])
	{
		mario.x += velocityX;
		isMoving = true;
	}
	else
	{
		isMoving = false;
	}

	if (isMoving)
	{
		RenderTextureClip(mario.x - camera.x, mario.y - camera.y, atlas, &marioRunningClips[(frame + 1) / 3]);
		++frame;

		if (frame / 3 >= MAX_FRAME)
		{
			frame = 0;
		}
	}
	else
	{
		RenderTextureClip(mario.x - camera.x, mario.y - camera.y, atlas, &marioRunningClips[0]);
	}
}

void ShutdownGame(GameState& state)
{
	for (auto& [key, val] : state.Instance->sprites)
	{
		SDL_DestroyTexture(val.data);
	}
}