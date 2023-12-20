#include "Game.h"

#include <iostream>
#include <memory>


#include "Texture.h"

static const uint32_t SPRITE_SIZE = 16;
static SDL_Rect marioRunningClip[3];
static const int MAX_FRAME = 3;
static int frame = 0;

void StartGame(GameState& state)
{
	if (!state.Instance)
	{
		state.Instance = std::make_unique<GameState>();
		std::cout << "Successully created a game instance\n";
	}

	// TODO: load texture from the specified path
	state.Instance->sprites["supermario_atlas"] = LoadTexture("assets/images/SuperMarioBrosAtlas.png");

	for (int i = 0; i < 3; i++)
	{
		marioRunningClip[i].x = SPRITE_SIZE * (i + 1);
		marioRunningClip[i].y = 0;
		marioRunningClip[i].w = SPRITE_SIZE;
		marioRunningClip[i].h = SPRITE_SIZE;
	}
}

void UpdateGame(GameState& state)
{
	Texture& marioTex = state.Instance->sprites["supermario_atlas"];

	const Uint8* input = SDL_GetKeyboardState(nullptr);

	if (input[SDL_SCANCODE_A])
	{
		marioTex.x -= 1;
	}
	else if (input[SDL_SCANCODE_D])
	{
		marioTex.x += 1;
	}

	marioTex.y = (728 / 2) - (marioTex.Height / 2);

	RenderTextureClip(marioTex, &marioRunningClip[frame / 3]);
	++frame;

	if (frame / 3 >= MAX_FRAME)
	{
		frame = 0;
	}
}

void ShutdownGame()
{
}