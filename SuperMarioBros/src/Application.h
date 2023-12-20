#pragma once

#include <SDL.h>
#include <SDL_image.h>

struct Application
{
	SDL_Window* window;
	SDL_Renderer* renderer;
};

void Run();

void* Renderer();
void* Window();