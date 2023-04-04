#pragma once

#include "Texture.h"
#include "SDL.h"
#include <vector>

namespace Window
{
	class Window
	{
	public:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		std::vector<Texture::Texture*> textures;
		Window();
		Window(const Window& other) = default;
		Window& operator=(const Window& other) = default;
		~Window() = default;
		void Update();
	};
}