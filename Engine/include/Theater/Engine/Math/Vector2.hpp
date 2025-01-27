#pragma once

#include <cmath>
#include "SIMD_Helpers.hpp"

#pragma region Vector2
namespace Theater
{
	struct ALIGN16 Vector2
	{
		float x;
		float y;

		constexpr Vector2() :
			x(0.f),
			y(0.f)
		{}

		constexpr explicit Vector2(float value) :
			x(value), y(value)
		{}

		constexpr explicit Vector2(const float* pVec) :
			x(pVec[0]), y(pVec[1])
		{}

		constexpr Vector2(float t_x, float t_y) :
			x(t_x), y(t_y)
		{}

		ALIGNED_MALLOC_OP(Vector2)

			constexpr Vector2& operator += (const Vector2& vec)
		{
			x += vec.x;
			y += vec.y;

			return *this;
		}

		constexpr Vector2& operator -= (const Vector2& vec)
		{
			x -= vec.x;
			y -= vec.y;

			return *this;
		}

		constexpr Vector2& operator *= (float scalar)
		{
			x *= scalar;
			y *= scalar;

			return *this;
		}

		constexpr Vector2& operator /= (float scalar)
		{
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr float& operator [](size_t i) { return (&x)[i]; }

		constexpr const float& operator [](size_t i) const { return (&x)[i]; }
	};

	constexpr inline Vector2 operator + (const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	constexpr inline Vector2 operator - (const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	constexpr inline Vector2 operator * (const Vector2& vec, float scalar)
	{
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	constexpr inline Vector2 operator / (const Vector2& vec, float scalar)
	{
		return Vector2(vec.x / scalar, vec.y / scalar);
	}

	constexpr inline Vector2 operator - (const Vector2& vec)
	{
		return Vector2(-vec.x, -vec.y);
	}

	constexpr inline float Dot(const Vector2& vec1, const Vector2& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	inline float Magnitude(const Vector2& vec)
	{
		return std::sqrtf(Dot(vec, vec));
	}

	inline Vector2 Normalize(const Vector2& vec)
	{
		return vec / Magnitude(vec);
	}
}
#pragma endregion


namespace Theater
{
	constexpr inline float PI = 3.1415927f;

	constexpr inline float NearZero = 0.00001f;

	constexpr inline float ToRad(float deg) { return deg * PI / 180.f; }
	constexpr inline float ToDeg(float radians) { return radians * 180.f / PI; }
}
