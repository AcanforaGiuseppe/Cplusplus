#pragma once
#include <vector>

class Map
{
public:
	Map(int InWidth, int InHeight);
	~Map();

	void Initialize();
	bool TryMoveUp();
	bool TryMoveDown();
	bool TryMoveLeft();
	bool TryMoveRight();
	bool CalculateDistance();
	bool CheckVictory();

	const int Width;
	const int Height;
	int PlayerPositionX;
	int PlayerPositionY;
	int ArtifactPositionX;
	int ArtifactPositionY;
	float DistToArtifact;
};