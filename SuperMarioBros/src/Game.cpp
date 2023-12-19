#include "Game.h"

#include <iostream>
#include <memory>
#include <unordered_map>

#include "Texture.h"


struct GameState
{
	GameState()
		: Instance(nullptr)
	{
	}
	~GameState() = default;

	std::unique_ptr<GameState> Instance;

	uint32_t Width;
	uint32_t Height;
	uint32_t Zoom;

	std::unordered_map<std::string, Texture> sprites;
};

static GameState state;

void GameStart(uint32_t width, uint32_t height, uint32_t zoom)
{
	if (!state.Instance)
	{
		state.Instance = std::make_unique<GameState>();
		std::cout << "Successully created a game instance\n";
	}

	state.Instance->Width = width;
	state.Instance->Height = height;
	state.Instance->Zoom = zoom;

	// TODO: load texture from the specified path
	state.Instance->sprites["supermario_atlas"] = LoadTexture("assets/images/SuperMarioBrosAtlas.png");
}

void GameShutdown()
{
}