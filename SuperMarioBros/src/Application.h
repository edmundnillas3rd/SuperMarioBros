#pragma once

#include <SDL.h>
#include <SDL_image.h>

struct Application
{
	Application() : Zoom(2.0f) {}
	SDL_Window* Window;
	SDL_Renderer* Renderer;

	float Zoom;
};

void Run();

void* Renderer();
void* Window();
Application GetApplicationProps();