#pragma once

#include "Vector2.hpp"


#pragma region Vector3
namespace Theater
{
	struct ALIGN16 Vector3
	{
		float x;
		float y;
		float z;
		float _w = 1.f;

		constexpr Vector3() :
			x(0.f),
			y(0.f),
			z(0.f)
		{}

		constexpr explicit Vector3(float value) :
			x(value), y(value), z(value)
		{}

		constexpr explicit Vector3(const float* pVec) :
			x(pVec[0]), y(pVec[1]), z(pVec[2])
		{}

		constexpr Vector3(float t_x, float t_y, float t_z) :
			x(t_x), y(t_y), z(t_z)
		{}

		constexpr explicit Vector3(const Vector2& vec) :
			Vector3(vec.x, vec.y, 0.f)
		{}

		ALIGNED_MALLOC_OP(Vector3)

			constexpr Vector3& operator += (const Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;

			return *this;
		}

		constexpr Vector3& operator -= (const Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;

			return *this;
		}

		constexpr Vector3& operator *= (float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		constexpr Vector3& operator /= (float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		constexpr float& operator [](size_t i) { return (&x)[i]; }
		constexpr const float& operator [](size_t i) const { return (&x)[i]; }
	};

	constexpr inline Vector3 operator + (const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	constexpr inline Vector3 operator - (const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	constexpr inline Vector3 operator * (const Vector3& vec, float scalar)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	constexpr inline Vector3 operator / (const Vector3& vec, float scalar)
	{
		return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
	}

	constexpr inline Vector3 operator - (const Vector3& vec)
	{
		return Vector3(-vec.x, -vec.y, -vec.z);
	}

	constexpr inline float Dot(const Vector3& vec1, const Vector3& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	constexpr inline Vector3 Cross(const Vector3& vec1, const Vector3& vec2)
	{
		return Vector3(
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x
		);
	}

	constexpr inline Vector3 Project(const Vector3& vec1, const Vector3& vec2)
	{
		return vec2 * (Dot(vec1, vec2) / Dot(vec2, vec2));
	}

	constexpr inline Vector3 Reject(const Vector3& vec1, const Vector3& vec2)
	{
		return (vec2 - vec2 * (Dot(vec1, vec2) / Dot(vec2, vec2)));
	}

	inline float Magnitude(const Vector3& vec)
	{
		return std::sqrtf(Dot(vec, vec));
	}

	inline Vector3 Normalize(const Vector3& vec)
	{
		return vec / Magnitude(vec);
	}
}
#pragma endregion

