#pragma once

#include "SDL.h"
#include "Vector2.h"

namespace Texture
{
	class Texture
	{
	public:
		Vector2::Vec2 position;
		int width;
		int height;

		SDL_Texture* _texture;
		Texture(const char* path, int width, int height, SDL_Window* window, SDL_Renderer* renderer);
		Texture(const Texture& other) = default;
		Texture& operator=(const Texture& other) = default;
		~Texture() = default;
	};
}