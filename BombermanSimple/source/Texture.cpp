#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

Texture::Texture::Texture(const char* path, int in_width, int in_height, SDL_Window* window, SDL_Renderer* renderer)
{
	width = in_width;
	height = in_height;

	int tex_width;
	int tex_height;
	int tex_channels;

	unsigned char* pixels = stbi_load(path, &tex_width, &tex_height, &tex_channels, 4);
	if (!pixels)
	{
		SDL_Log("Unable to open image");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, tex_width, tex_height);
	if (!_texture)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		free(pixels);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	SDL_UpdateTexture(_texture, NULL, pixels, tex_width * tex_channels);
	free(pixels);
}