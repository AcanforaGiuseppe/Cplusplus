#include "map.h"
#include <cstdlib>
#include <math.h>

Map::Map(int InWidth, int InHeight) : Width(InWidth), Height(InHeight)
{
	Initialize();
}

Map::~Map()
{ }

void Map::Initialize()
{
	// Generate random player position
	PlayerPositionX = rand() % Width;
	PlayerPositionY = rand() % Height;

	// Generate random artifact position
	while (true)
	{
		ArtifactPositionX = rand() % Width;
		ArtifactPositionY = rand() % Height;

		if (PlayerPositionX != ArtifactPositionX && PlayerPositionY != ArtifactPositionY)
			break;
	}

	CalculateDistance();
}

bool Map::CalculateDistance()
{
	int x = abs(PlayerPositionX - ArtifactPositionX);
	int y = abs(PlayerPositionY - ArtifactPositionY);
	int oldDist = DistToArtifact;
	DistToArtifact = x * x + y * y;

	return DistToArtifact > oldDist;
}

bool Map::CheckVictory()
{
	return (PlayerPositionX == ArtifactPositionX && PlayerPositionY == ArtifactPositionY);
}

bool Map::TryMoveUp()
{
	if (PlayerPositionY >= Height)
		return false;

	PlayerPositionY++;
	return true;
}

bool Map::TryMoveDown()
{
	if (PlayerPositionY <= 0)
		return false;

	PlayerPositionY--;
	return true;
}

bool Map::TryMoveLeft()
{
	if (PlayerPositionX <= 0)
		return false;

	PlayerPositionX--;
	return true;
}

bool Map::TryMoveRight()
{
	if (PlayerPositionX >= Width)
		return false;

	PlayerPositionX++;
	return true;
}