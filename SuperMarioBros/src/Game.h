#pragma once
#include <cstdint>
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

void GameStart(GameState& state);
void GameShutdown();