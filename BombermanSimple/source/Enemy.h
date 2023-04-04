#pragma once

#include "Vector2.h"

namespace Enemy
{
	enum Enemy_Type
	{
		BAT,
		BALOON
	};

	class Enemy
	{
	public:
		Vector2::Vec2 position;
		const Enemy_Type type;

		Enemy(Vector2::Vec2 position, const Enemy_Type type);
		Enemy(const Enemy& other) = default;
		Enemy& operator=(const Enemy& other) = default;
		~Enemy() = default;
	};
}