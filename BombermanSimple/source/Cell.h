#pragma once

namespace Cell
{
	enum Cell_Type
	{
		GROUND,
		WALL,
		UNDESTROYABLE_WALL,
		TELEPORT
	};

	enum Bonus_Type
	{
		NONE,
		SPEED,
		BOMB2,
		BOMB3
	};

	class Cell
	{
	public:
		const Cell_Type cell_type;
		const Bonus_Type bonus_type;

		Cell(const Cell_Type in_cell_type, const Bonus_Type in_bonus_type);
		Cell(const Cell& other) = default;
		Cell& operator=(const Cell& other) = default;
		~Cell() = default;
	};
}