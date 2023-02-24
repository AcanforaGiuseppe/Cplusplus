#include "aiv_math.h"

aiv_math::Vector2::Vector2(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
}

aiv_math::Vector2& aiv_math::Vector2::operator=(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;

	return *this;
}

aiv_math::Vector2::Vector2(const float x, const float y)
{
	this->x = x;
	this->y = y;
}

aiv_math::Vector2 aiv_math::Vector2::operator+(const Vector2& other)
{
	Vector2 result(other.x + this->x, other.y + this->y);
	return result;
}

aiv_math::Vector2 aiv_math::Vector2::operator*(const Vector2& other)
{
	return Vector2(other.x * this->x, other.y * this->y);
}

aiv_math::Vector2 aiv_math::Vector2::operator*(const float value)
{
	return Vector2(this->x * value, this->y * value);
}

aiv_math::Vector2::operator aiv_math::Vector3()
{
	Vector3 newVector2(this->x, this->y, 0);
	return newVector2;
}

aiv_math::Vector3::Vector3(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}