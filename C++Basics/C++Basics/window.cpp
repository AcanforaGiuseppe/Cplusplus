#include "window.h"
#include "SDL.h"

game::Window::Window(const std::string& title, const int width, const int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		failed = true;
		return;
	}

	window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, 0);
	if (!window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		failed = true;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		failed = true;
		return;
	}
}

bool game::Window::IsFailed() const
{
	return failed;
}

void game::Window::Run(ITickable& tickable) const
{
	bool running = true;
	while (running)
	{
		StartFrame(running);
		tickable.Tick(1.f / 60.f);
		EndFrame();
	}
}

void game::Window::Run(std::shared_ptr<ITickable> tickable) const
{
	//return functor 1.f/60.f
	Run(std::bind(&game::ITickable::Tick, tickable.get(), std::placeholders::_1));
	Run(*tickable.get());
}

void game::Window::Run(std::unique_ptr<ITickable> tickable) const
{
	Run(*tickable.get());
}

void game::Window::Run(std::function<void(const float)> tick) const
{
	bool running = true;
	while (running)
	{
		StartFrame(running);
		tick(1.f / 60.f);
		EndFrame();
	}
}

void game::Window::StartFrame(bool running) const
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;
	}
}

void game::Window::EndFrame() const
{
	//tickable.Tick(1.f / 60.f);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	//SDL_RenderCopy(renderer, texture->sdl_texture, NULL, &target_rect);
	SDL_RenderPresent(renderer);
}

game::Window::~Window()
{
	if (renderer)
		SDL_DestroyRenderer(renderer);

	if (window)
		SDL_DestroyWindow(window);

	if (SDL_WasInit(0) != 0)
		SDL_Quit();
}