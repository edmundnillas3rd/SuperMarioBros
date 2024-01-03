#include "Texture.h"

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Application.h"

Texture LoadTexture(const std::string& path)
{
	Texture tex;

	SDL_Surface* loadedSurface = nullptr;

	loadedSurface = IMG_Load(path.c_str());

	if (path.empty())
	{
		std::cout << "Please specify a path for a texture\n";
		return tex;
	}

	if (!loadedSurface)
	{
		std::cout << "Unable to load image from " << path << "\n";
		return tex;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x92, 0x90, 0xFF));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(reinterpret_cast<SDL_Renderer*>(Renderer()), loadedSurface);
	SDL_FreeSurface(loadedSurface);

	if (!texture)
	{
		std::cout << "Unable to successfully create a texture\n";
		std::cout << "SDL Error: " << SDL_GetError() << "\n";
		return tex;
	}

	tex.data = texture;
	tex.Width = loadedSurface->w;
	tex.Height = loadedSurface->h;
	tex.x = 0;
	tex.y = 0;

	return tex;
}

void RenderTextureClip(float x, float y, Texture texture, SDL_Rect* clip)
{
	float zoom = GetApplicationProps().Zoom;
	SDL_FRect renderQuad = { x, y, texture.Width * zoom, texture.Height * zoom};

	if (clip)
	{
		renderQuad.w = clip->w * zoom;
		renderQuad.h = clip->h * zoom;
	}

	auto* renderer = reinterpret_cast<SDL_Renderer*>(Renderer());

#ifdef _DEBUG
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRectF(renderer, &renderQuad);
#endif

	SDL_RenderCopyF(renderer, texture.data, clip, &renderQuad);
}
