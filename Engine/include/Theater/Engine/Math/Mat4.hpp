#pragma once

#include "Vector4.hpp"

#pragma region Matrix4
namespace Theater
{
	struct ALIGN16 Matrix4
	{
	public:

		constexpr Matrix4() = default;
		constexpr Matrix4(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
		{
			m_Mat[0][0] = col1.x;
			m_Mat[0][1] = col1.y;
			m_Mat[0][2] = col1.z;
			m_Mat[0][3] = col1.w;

			m_Mat[1][0] = col2.x;
			m_Mat[1][1] = col2.y;
			m_Mat[1][2] = col2.z;
			m_Mat[1][3] = col2.w;

			m_Mat[2][0] = col3.x;
			m_Mat[2][1] = col3.y;
			m_Mat[2][2] = col3.z;
			m_Mat[2][3] = col3.w;

			m_Mat[3][0] = col4.x;
			m_Mat[3][1] = col4.y;
			m_Mat[3][2] = col4.z;
			m_Mat[3][3] = col4.w;
		}

		ALIGNED_MALLOC_OP(Matrix4)

	public:

		constexpr const float* GetFloatPtr() const { return m_Mat[0]; }

	public:

		constexpr const float& operator ()(size_t i, size_t j) const { return m_Mat[j][i]; }
		constexpr float& operator ()(size_t i, size_t j) { return m_Mat[j][i]; }

		Vector4& operator [](size_t j) { return *reinterpret_cast<Vector4*>(m_Mat[j]); }
		const Vector4& operator [](size_t j) const { return *reinterpret_cast<const Vector4*>(m_Mat[j]); }

	private:
		float m_Mat[4][4]{ 0.f };
	};

	inline Matrix4 operator * (const Matrix4& lhs, const Matrix4& rhs)
	{
		const Vector4 cLeftRow1(lhs(0, 0), lhs(0, 1), lhs(0, 2), lhs(0, 3));
		const Vector4 cLeftRow2(lhs(1, 0), lhs(1, 1), lhs(1, 2), lhs(1, 3));
		const Vector4 cLeftRow3(lhs(2, 0), lhs(2, 1), lhs(2, 2), lhs(2, 3));
		const Vector4 cLeftRow4(lhs(3, 0), lhs(3, 1), lhs(3, 2), lhs(3, 3));

		const Vector4& cRightCol1 = rhs[0];
		const Vector4& cRightCol2 = rhs[1];
		const Vector4& cRightCol3 = rhs[2];
		const Vector4& cRightCol4 = rhs[3];

		const Vector4 cCol1(
			Dot(cLeftRow1, cRightCol1),
			Dot(cLeftRow2, cRightCol1),
			Dot(cLeftRow3, cRightCol1),
			Dot(cLeftRow4, cRightCol1)
		);

		const Vector4 cCol2(
			Dot(cLeftRow1, cRightCol2),
			Dot(cLeftRow2, cRightCol2),
			Dot(cLeftRow3, cRightCol2),
			Dot(cLeftRow4, cRightCol2)
		);

		const Vector4 cCol3(
			Dot(cLeftRow1, cRightCol3),
			Dot(cLeftRow2, cRightCol3),
			Dot(cLeftRow3, cRightCol3),
			Dot(cLeftRow4, cRightCol3)
		);

		const Vector4 cCol4(
			Dot(cLeftRow1, cRightCol4),
			Dot(cLeftRow2, cRightCol4),
			Dot(cLeftRow3, cRightCol4),
			Dot(cLeftRow4, cRightCol4)
		);

		return Matrix4(cCol1, cCol2, cCol3, cCol4);
	}

	inline Vector4 operator * (const Matrix4& lhs, const Vector4& rhs)
	{
		return Vector4(
			Dot(Vector4(lhs(0, 0), lhs(0, 1), lhs(0, 2), lhs(0, 3)), rhs),
			Dot(Vector4(lhs(1, 0), lhs(1, 1), lhs(1, 2), lhs(1, 3)), rhs),
			Dot(Vector4(lhs(2, 0), lhs(2, 1), lhs(2, 2), lhs(2, 3)), rhs),
			Dot(Vector4(lhs(3, 0), lhs(3, 1), lhs(3, 2), lhs(3, 3)), rhs)
		);
	}

	inline Vector4 operator / (const Matrix4& lhs, const Vector4& rhs)
	{
		return lhs * Vector4(1.f / rhs.x, 1.f / rhs.y, 1.f / rhs.z, 1.f / rhs.w);
	}

	inline Matrix4 Translate(const Matrix4& mat, const Vector3& vec)
	{
		Matrix4 result(mat);
		result[3] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3] * vec[3];
		return result;
	}

	inline Matrix4 Scale(const Matrix4& mat, const Vector3& factor)
	{
		Matrix4 result;
		result[0] = mat[0] * factor[0];
		result[1] = mat[1] * factor[1];
		result[2] = mat[2] * factor[2];
		result[3] = mat[3];

		return result;
	}

	inline Matrix4 RotateX(const Matrix4& mat, float angle)
	{
		return mat * Matrix4(
			Vector4(1.f, 0.f, 0.f, 0.f),
			Vector4(0.f, std::cos(angle), std::sin(angle), 0.f),
			Vector4(0.f, -std::sin(angle), std::cos(angle), 0.f),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);
	}

	inline Matrix4 RotateY(const Matrix4& mat, float angle)
	{
		return mat * Matrix4(
			Vector4(std::cos(angle), 0.f, -std::sin(angle), 0.f),
			Vector4(0.f, 1.f, 0.f, 0.f),
			Vector4(std::sin(angle), 0.f, std::cos(angle), 0.f),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);
	}

	inline Matrix4 RotateZ(const Matrix4& mat, float angle)
	{
		return mat * Matrix4(
			Vector4(std::cos(angle), std::sin(angle), 0.f, 0.f),
			Vector4(-std::sin(angle), std::cos(angle), 0.f, 0.f),
			Vector4(0.f, 0.f, 1.f, 0.f),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);
	}

	inline Matrix4 BuildPerspective(float FOV, float aspectRatio, float zNear, float zFar)
	{
		const float TanHalfFOV = std::tan(FOV / 2.0f);

		return Matrix4(
			Vector4(1.f / (aspectRatio * TanHalfFOV), 0.f, 0.f, 0.f),
			Vector4(0.f, 1.f / TanHalfFOV, 0.f, 0.f),
			Vector4(0.f, 0.f, -(zFar + zNear) / (zFar - zNear), -1.f),
			Vector4(0.f, 0.f, -2.f * zFar * zNear / (zFar - zNear), 0.f)
		);
	}
}
#pragma endregion

namespace Theater
{
	constexpr inline Matrix4 IMat4(Vector4(1.f, 0.f, 0.f, 0.f), Vector4(0.f, 1.f, 0.f, 0.f), Vector4(0.f, 0.f, 1.f, 0.f), Vector4(0.f, 0.f, 0.f, 1.f));
}