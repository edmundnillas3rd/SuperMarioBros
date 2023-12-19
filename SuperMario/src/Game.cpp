#include "Game.h"

#include <iostream>
#include <memory>


struct GameState
{
	std::unique_ptr<GameState> Instance;

	uint32_t Width;
	uint32_t Height;
	uint32_t Zoom;
};

static GameState state;

void GameStart(uint32_t width, uint32_t height, uint32_t zoom)
{
	state = { 0 };

	if (!state.Instance)
	{
		std::cout << "Successully created a game instance\n";
		state.Instance = std::make_unique<GameState>();
	}

	state.Instance->Width = width;
	state.Instance->Height = height;
	state.Instance->Zoom = zoom;

}

void GameShutdown()
{

}