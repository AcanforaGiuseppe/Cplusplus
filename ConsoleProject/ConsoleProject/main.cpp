#include "map.h"
#include <iostream>
#include <string>
#include <conio.h>

void PrintPlayerPosition(Map& m)
{
	std::cout << "You're here (" << m.PlayerPositionX << ", " << m.PlayerPositionY << "), Where do you want to go?" << std::endl;
}

void PrintCantMove()
{
	std::cout << "You can't go there!" << std::endl;
}

void PrintSuccessfulMove(int orientation)
{
	if (orientation == 0)
		std::cout << "You moved up!" << std::endl;

	if (orientation == 1)
		std::cout << "You moved down!" << std::endl;

	if (orientation == 2)
		std::cout << "You moved right!" << std::endl;

	if (orientation == 3)
		std::cout << "You moved left!" << std::endl;
}

int main()
{
	srand(time(NULL));

	int width = 5;
	int height = 5;

	Map m = Map(width, height);
	//std::cout << "MAP IS " << width << "x" << height << std::endl;
	PrintPlayerPosition(m);

	std::string input;
	for (;;)
	{
		// Read input
		std::cin >> input;
		std::cout << std::endl;

		bool moveSuccessful = false;
		int orientation = 0;

		if (input.compare("w") == 0 || input.compare("W") == 0)
		{
			moveSuccessful = m.TryMoveUp();
			orientation = 0;
		}

		if (input.compare("s") == 0 || input.compare("S") == 0)
		{
			moveSuccessful = m.TryMoveDown();
			orientation = 1;
		}

		if (input.compare("a") == 0 || input.compare("A") == 0)
		{
			moveSuccessful = m.TryMoveLeft();
			orientation = 3;
		}

		if (input.compare("d") == 0 || input.compare("D") == 0)
		{
			moveSuccessful = m.TryMoveRight();
			orientation = 2;
		}

		if (input.compare("q") == 0 || input.compare("Q") == 0)
		{
			std::cout << "Quitting game..." << std::endl;
			break;
		}

		if (!moveSuccessful)
		{
			PrintCantMove();
		}
		else
		{
			PrintSuccessfulMove(orientation);

			if (m.CheckVictory())
			{
				std::cout << "You found the artifact!!" << std::endl;
				break;
			}

			if (!m.CalculateDistance())
				std::cout << "You're getting closer..." << std::endl;
			else
				std::cout << "You're moving away..." << std::endl;
		}

		PrintPlayerPosition(m);
	}

	_getch();
}