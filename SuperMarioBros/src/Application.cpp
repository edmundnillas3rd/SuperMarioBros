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

	int imgInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgInitFlags) == 0)
	{
		std::cout << "Failed to initialized image library\n";
	}

	const uint32_t SCREEN_WIDTH = 1366;
	const uint32_t SCREEN_HEIGHT = 720;
	app.Window = SDL_CreateWindow("Super Mario Bros.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
	app.Renderer = SDL_CreateRenderer(app.Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (app.Window && app.Renderer)
	{
		std::cout << "Window and Renderer successfully created\n";
	}

	bool running = true;
	SDL_Event event;

	GameState state;

	StartGame(state);

	float currentTime = 0.0f;

	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;

	while (running)
	{
		last = now;
		now = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&event))
		{

			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
#ifdef _DEBUG
				float stepValue = 1.0f;
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					app.Zoom += stepValue;
					break;
				case SDLK_DOWN:
					app.Zoom -= stepValue;
					break;
				}
#endif
				break;
			}

		}

		UpdateGame(state);

		if (!running) break;

		SDL_SetRenderDrawColor(app.Renderer, 0x22, 0x44, 0x43, 0xFF);
		SDL_RenderClear(app.Renderer);
		RenderGame(state);
		SDL_RenderPresent(app.Renderer);
		state.DeltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
	}

	ShutdownGame(state);

	SDL_DestroyRenderer(app.Renderer);
	SDL_DestroyWindow(app.Window);

	IMG_Quit();
	SDL_Quit();
}

void* Renderer()
{
	return app.Renderer;
}

void* Window()
{
	return app.Window;
}

Application GetApplicationProps()
{
	return app;
}
