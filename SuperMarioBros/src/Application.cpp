#include "Application.h"
#include <iostream>

#include "Game.h"

static Application app;

void Run()
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
	app.window = SDL_CreateWindow("Super Mario Bros.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	if (app.window && app.renderer)
	{
		std::cout << "Window and Renderer successfully created\n";
	}

	bool running = true;
	SDL_Event event;

	GameState state;

	GameStart(state);

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

		SDL_RenderPresent(app.renderer);
		SDL_SetRenderDrawColor(app.renderer, 0x22, 0x44, 0x43, 0xFF);
		SDL_RenderClear(app.renderer);
	}

	GameShutdown();

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	IMG_Quit();
	SDL_Quit();
}

void* Renderer()
{
	return app.renderer;
}

void* Window()
{
	return app.window;
}
