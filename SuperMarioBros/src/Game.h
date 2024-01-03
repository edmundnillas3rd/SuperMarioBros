#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Texture.h"

struct GameState
{
	GameState()
		: Instance(nullptr), DeltaTime(0.0)
	{
	}
	~GameState() = default;

	std::unique_ptr<GameState> Instance;

	uint32_t Width;
	uint32_t Height;

	std::unordered_map<std::string, Texture> sprites;
	double DeltaTime;
};

void StartGame(GameState& state);
void UpdateGame(GameState& state);
void RenderGame(GameState& state);
void ShutdownGame(GameState& state);