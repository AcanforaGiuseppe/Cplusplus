#pragma once

namespace Vector2
{
	struct Vec2
	{
	public:
		float x;
		float y;

		Vec2(float x = 0, float y = 0);
		Vec2(const Vec2& other) = default;
		Vec2& operator=(const Vec2& other) = default;
		~Vec2() = default;
	};
}