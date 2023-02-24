namespace aiv_math
{
	struct Vector3
	{
		float x = 0;
		float y = 0;
		float z = 0;

		Vector3() = default;
		~Vector3() = default;
		Vector3(const float x, const float y, const float z);
	};

	struct Vector2
	{
		float x = 0;
		float y = 0;

		Vector2() = default;
		~Vector2() = default;
		Vector2(const Vector2& other);
		Vector2& operator=(const Vector2& other);
		Vector2(const float x, const float y);
		Vector2 operator+(const Vector2& other);
		Vector2 operator*(const Vector2& other);
		Vector2 operator*(const float value);
		operator Vector3();
	};

}