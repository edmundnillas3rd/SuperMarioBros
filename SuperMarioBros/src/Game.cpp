#include "Game.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Application.h"
#include "Texture.h"
#include "Tilemap.h"
#include "AABB.h"
#include "Pathfinding.h"

enum class TileType
{
	NONE				= -1,
	BRICK_BASE			=  0,
	BRICK_SOLID_SHINE	=  1,
	BRICK_SOLID			=  2,
	BRICK_HARD_SOLID	=  3,
	FLAG_POLE_HEAD		=  4,
	FLAG_POLE_BODY		=  5
};

static const uint32_t SPRITE_SIZE = 16;
static uint32_t RESIZE_SPRITE_SIZE;
static SDL_FRect camera;

// Tile
static std::vector<SDL_Rect> tileClips;

enum class Direction 
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct Mario
{
	float x;
	float y;
	Direction direction;
};

static bool levelFinished = false;
static float floorCoordY = 0.0f;

static bool onGround = false;
static bool isMoving = false;
static const uint32_t SPEED = 200;
static float velocityX = 0.0f;
static float velocityY = 0.0f;
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
	for (int i = 0; i < 6; i++)
	{
		const int TILE_SPRITE_OFFSET = (i);
		SDL_Rect tileClip = { 0 };
		tileClip.x = SPRITE_SIZE * TILE_SPRITE_OFFSET;
		tileClip.y = SPRITE_SIZE;
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
	
	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);
	camera = { 0, 0, (float)w, (float)h };

	// Should resize the sprite after loading the clip textures
	RESIZE_SPRITE_SIZE = 16 * GetApplicationProps().Zoom;

	// Mario should be placed right above the level
	// mario.y = 382 - SPRITE_SIZE;
	floorCoordY = 765 - RESIZE_SPRITE_SIZE * 2;
	mario.x = RESIZE_SPRITE_SIZE;
	mario.y = floorCoordY;
	mario.direction = Direction::NONE;
	

	Tile tileProps = {
		0, 0, (float)RESIZE_SPRITE_SIZE, (float)RESIZE_SPRITE_SIZE
	};

	LoadTilemap("Level-1-1", "assets/maps/level-1-1.txt", tileProps, [](char tile) -> int {
		TileType type = TileType::NONE;

		switch (tile)
		{
		case '#':
			type = TileType::BRICK_BASE;
			break;
		case 's':
			type = TileType::BRICK_HARD_SOLID;
			break;
		case 'o':
			type = TileType::FLAG_POLE_HEAD;
			break;
		case '|':
			type = TileType::FLAG_POLE_BODY;
			break;
		}

		return (int)type;
	});
}

void UpdateGame(GameState& state)
{
	SDL_FRect marioCollider = { mario.x, mario.y, (float)RESIZE_SPRITE_SIZE, (float)RESIZE_SPRITE_SIZE };

	int w, h;
	SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(Window()), &w, &h);

	camera.x = mario.x - w / 2;
	camera.y = mario.y - h / 2;

	if (levelFinished)
	{
		if (mario.x + 1.0f >= 1000.0f)
		{
			isMoving = false;
			return;
		}

		if (mario.y + 1.0f >= floorCoordY)
		{
			const float SPEED_MODIFIER = 200.0f;
			mario.x = MoveTowards(mario.x, 1000.0f, state.DeltaTime * SPEED_MODIFIER);
			return;
		}

		mario.y = MoveTowards(mario.y + 2.0f, floorCoordY, state.DeltaTime * 20.0f);
		return;
	}

	CheckTilemapCollision(marioCollider, [&state](Tile tile) {
		switch ((TileType)tile.Type)
		{

		case TileType::FLAG_POLE_BODY:
		case TileType::FLAG_POLE_HEAD:
			levelFinished = true;
			onGround = true;
			mario.y = tile.y;

			break;

		case TileType::BRICK_BASE:

			// Direction currentDirection = mario.direction;
			// if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT)
			// {
			// 	mario.x -= velocityX;
			// }

			if (mario.y + RESIZE_SPRITE_SIZE > tile.y)
			{
				onGround = true;
				velocityY = 0.0f;
				mario.y += velocityY * state.DeltaTime;
			}
			else
			{
				onGround = false;
			}
			break;
		}
	});

	// Mario
	const Uint8* input = SDL_GetKeyboardState(nullptr);

	// const float GRAVITY = 9.8f;
	// const float MASS = 10.0f;
	// const float FORCE = onGround ? 150.0f : GRAVITY;
	// const float ACCELERATION = (FORCE / MASS);
	velocityX = SPEED * state.DeltaTime;

	if (onGround && input[SDL_SCANCODE_SPACE])
	{
		onGround = false;
		velocityY = -500.0f;
	}

	if (!onGround)
	{
		const float GRAVITY = 950.0f;
		velocityY += GRAVITY * state.DeltaTime;
		mario.y += velocityY * state.DeltaTime;
	}

	if (input[SDL_SCANCODE_A])
	{
		mario.x -= velocityX;
		isMoving = true;
		mario.direction = Direction::LEFT;
	}
	else if (input[SDL_SCANCODE_D])
	{
		mario.x += velocityX;
		isMoving = true;
		mario.direction = Direction::RIGHT;
	}
	else
	{
		isMoving = false;
		mario.direction = Direction::NONE;
	}
}

void RenderGame(GameState& state)
{
	Texture atlas = state.Instance->sprites["supermario_atlas"];
	RenderTilemap(camera, atlas, tileClips);

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
