#pragma once

namespace GameMode
{
	class GameMode
	{
	public:
		unsigned int timer;

		GameMode(unsigned int timer = 60);
		GameMode(const GameMode& other) = default;
		GameMode& operator=(const GameMode& other) = default;
		~GameMode() = default;
	};
}