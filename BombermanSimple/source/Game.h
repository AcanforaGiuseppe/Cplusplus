#pragma once

#include "Window.h"

namespace Game
{
	class Game
	{
	public:
		Game();
		Game(Game& other) = default;
		Game& operator=(Game& other) = default;
		~Game() = default;
	};
}