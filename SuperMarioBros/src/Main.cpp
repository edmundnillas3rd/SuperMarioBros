#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Game.h"

static void Run()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialized SDL\n";
	}

	int imgInitFlags = IMG_INIT_JPG | IMG_INIT_JPG;
	if (IMG_Init(imgInitFlags) == 0)
	{
		std::cout << "Failed to initialized image library\n";
	}

	const uint32_t SCREEN_WIDTH = 1366;
	const uint32_t SCREEN_HEIGHT = 720;
	SDL_Window* window = SDL_CreateWindow("Super Mario Bros.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window && renderer)
	{
		std::cout << "Window and Renderer successfully created\n";
	}

	bool running = true;
	SDL_Event event;

	GameStart(SCREEN_WIDTH, SCREEN_HEIGHT);

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		if (!running) break;

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0x22, 0x44, 0x43, 0xFF);
		SDL_RenderClear(renderer);
	}

	GameShutdown();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char** argv)
{
	Run();
	return 0;
}