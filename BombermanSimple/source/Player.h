#pragma once

#include "Vector2.h"
#include "Texture.h"

namespace Player
{
	class Player
	{
	public:
		Vector2::Vec2 position;
		Texture::Texture* texture;
		unsigned int number_of_lifes;
		unsigned int number_of_bombs;
		unsigned int score;
		unsigned int speed;

		Player();
		Player(const Player& other) = default;
		Player& operator=(const Player& other) = default;
		~Player();
		void Update();
	};
}