#pragma once

#include "Vector2.h"

namespace Bomb
{
	class Bomb
	{
	public:
		Vector2::Vec2 position;
		unsigned int range;
		unsigned int cooldown;

		Bomb(Vector2::Vec2 position, unsigned int range, unsigned int cooldown);
		Bomb(const Bomb& other) = default;
		Bomb& operator=(const Bomb& other) = default;
		~Bomb() = default;
	};
}