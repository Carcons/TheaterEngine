#pragma once

#include "Vector3.hpp"

#pragma region Vector4
namespace Theater
{
	struct ALIGN16 Vector4
	{
		float x;
		float y;
		float z;
		float w;

		constexpr Vector4() :
			x(0.f),
			y(0.f),
			z(0.f),
			w(0.f)
		{}

		constexpr explicit Vector4(float value) :
			x(value), y(value), z(value), w(value)
		{}

		constexpr explicit Vector4(const float* vec) :
			x(vec[0]), y(vec[1]), z(vec[2]), w(vec[3])
		{}

		constexpr Vector4(float t_x, float t_y, float t_z, float t_w) :
			x(t_x), y(t_y), z(t_z), w(t_w)
		{}

		constexpr explicit Vector4(const Vector3& vec) :
			Vector4(vec.x, vec.y, vec.z, vec._w)
		{}

		ALIGNED_MALLOC_OP(Vector4)

		constexpr Vector4& operator += (const Vector4& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;

			return *this;
		}

		constexpr Vector4& operator -= (const Vector4& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;

			return *this;
		}

		constexpr Vector4& operator *= (float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;

			return *this;
		}

		constexpr Vector4& operator /= (float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;

			return *this;
		}

		constexpr float& operator [](size_t i) { return (&x)[i]; }

		constexpr const float& operator [](size_t i) const { return (&x)[i]; }
	};

	constexpr inline Vector4 operator + (const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	constexpr inline Vector4 operator - (const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	constexpr inline Vector4 operator * (const Vector4& vec, float scalar)
	{
		return Vector4(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
	}

	constexpr inline Vector4 operator / (const Vector4& vec, float scalar)
	{
		return Vector4(vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar);
	}

	constexpr inline Vector4 operator - (const Vector4& vec)
	{
		return Vector4(-vec.x, -vec.y, -vec.z, -vec.w);
	}

	constexpr inline float Dot(const Vector4& vec1, const Vector4& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}

	inline float Magnitude(const Vector4& vec)
	{
		return std::sqrtf(Dot(vec, vec));
	}

	inline Vector4 Normalize(const Vector4& vec)
	{
		return vec / Magnitude(vec);
	}
}
#pragma endregion
