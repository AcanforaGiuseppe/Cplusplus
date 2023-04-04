#pragma once

#include "Window.h"

Window::Window::Window()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());

	_window = SDL_CreateWindow("SDL is active!", 100, 100, 512, 512, 0);
	if (!_window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		SDL_Quit();
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}
}

void Window::Window::Update()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	for (Texture::Texture* texture : textures)
	{
		if (texture)
		{
			SDL_Rect target_rect = { static_cast<int>(texture->position.x), static_cast<int>(texture->position.y), texture->width, texture->height };
			SDL_RenderCopy(_renderer, texture->_texture, NULL, &target_rect);
		}
	}

	SDL_RenderPresent(_renderer);
}