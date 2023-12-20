#pragma once
#include <string>
#include <memory>

#include <SDL.h>

struct Texture
{
	Texture() 
		: data(nullptr) 
	{ 
	}
	~Texture() = default;

	SDL_Texture* data;

	uint32_t x;
	uint32_t y;
	uint32_t Width;
	uint32_t Height;
};

Texture LoadTexture(const std::string& path);
void RenderTextureClip(float x, float y, Texture texture, SDL_Rect* clip = nullptr);
