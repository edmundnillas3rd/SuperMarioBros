#include "Game.h"

#include <iostream>
#include <memory>

#include "Texture.h"

void GameStart(GameState& state)
{
	if (!state.Instance)
	{
		state.Instance = std::make_unique<GameState>();
		std::cout << "Successully created a game instance\n";
	}

	// TODO: load texture from the specified path
	state.Instance->sprites["supermario_atlas"] = LoadTexture("assets/images/SuperMarioBrosAtlas.png");
}

void GameShutdown()
{
}