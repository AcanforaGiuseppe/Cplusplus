#include "Game.h"
#include "Player.h"

Game::Game::Game()
{
	Window::Window window;
	Player::Player player;
	player.position = Vector2::Vec2(128, 128);
	player.texture = new Texture::Texture("photo.png", 128, 128, window._window, window._renderer);
	window.textures.push_back(player.texture);

	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
		}

		SDL_PumpEvents();

		const Uint8* keys = SDL_GetKeyboardState(NULL);
		player.position.x += keys[SDL_SCANCODE_RIGHT];
		player.position.x -= keys[SDL_SCANCODE_LEFT];
		player.position.y += keys[SDL_SCANCODE_DOWN];
		player.position.y -= keys[SDL_SCANCODE_UP];

		player.Update();
		window.Update();
	}
}