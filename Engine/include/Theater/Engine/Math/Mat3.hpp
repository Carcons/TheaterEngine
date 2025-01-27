#pragma once

#include "Vector3.hpp"

#pragma region Matrix3
namespace Theater
{
	struct ALIGN16 Matrix3
	{
	public:

		constexpr Matrix3() = default;
		constexpr Matrix3(const Vector3& col1, const Vector3& col2, const Vector3& col3)
		{
			m_Mat[0][0] = col1.x;
			m_Mat[0][1] = col1.y;
			m_Mat[0][2] = col1.z;

			m_Mat[1][0] = col2.x;
			m_Mat[1][1] = col2.y;
			m_Mat[1][2] = col2.z;

			m_Mat[2][0] = col3.x;
			m_Mat[2][1] = col3.y;
			m_Mat[2][2] = col3.z;
		}

		ALIGNED_MALLOC_OP(Matrix3)

	public:

		constexpr float& operator ()(size_t i, size_t j) { return m_Mat[j][i]; }

		constexpr const float& operator ()(size_t i, size_t j) const { return m_Mat[j][i]; }

		Vector3& operator [](size_t j) { return *reinterpret_cast<Vector3*>(m_Mat[j]); }
		const Vector3& operator [](size_t j) const { return *reinterpret_cast<const Vector3*>(m_Mat[j]); }

	public:

		const float* GetFloatPtr() const { return m_Mat[0]; }

	private:

		float m_Mat[3][3]{ 0.f };
	};

	inline Matrix3 Inverse(const Matrix3& mat)
	{
		const Vector3& cA = mat[0];
		const Vector3& cB = mat[1];
		const Vector3& cC = mat[2];

		const Vector3 cRow0 = Cross(cB, cC);
		const Vector3 cRow1 = Cross(cC, cA);
		const Vector3 cRow2 = Cross(cA, cB);

		const float cInvDet = 1.f / Dot(cRow2, cC);

		const Vector3 cColResult1 = Vector3(cRow0.x, cRow1.x, cRow2.x) * cInvDet;
		const Vector3 cColResult2 = Vector3(cRow0.y, cRow1.y, cRow2.y) * cInvDet;
		const Vector3 cColResult3 = Vector3(cRow0.z, cRow1.z, cRow2.z) * cInvDet;

		return Matrix3(cColResult1, cColResult2, cColResult3);
	}

	inline Matrix3 operator * (const Matrix3& lhs, const Matrix3& rhs)
	{
		const Vector3 cLeftRow1(lhs(0, 0), lhs(0, 1), lhs(0, 2));
		const Vector3 cLeftRow2(lhs(1, 0), lhs(1, 1), lhs(1, 2));
		const Vector3 cLeftRow3(lhs(2, 0), lhs(2, 1), lhs(2, 2));

		const Vector3& cRightCol1 = rhs[0];
		const Vector3& cRightCol2 = rhs[1];
		const Vector3& cRightCol3 = rhs[2];

		const Vector3 cCol1(
			Dot(cLeftRow1, cRightCol1),
			Dot(cLeftRow2, cRightCol1),
			Dot(cLeftRow3, cRightCol1)
		);

		const Vector3 cCol2(
			Dot(cLeftRow1, cRightCol2),
			Dot(cLeftRow2, cRightCol2),
			Dot(cLeftRow3, cRightCol2)
		);

		const Vector3 cCol3(
			Dot(cLeftRow1, cRightCol3),
			Dot(cLeftRow2, cRightCol3),
			Dot(cLeftRow3, cRightCol3)
		);

		return Matrix3(cCol1, cCol2, cCol3);
	}

	constexpr inline Vector3 operator * (const Matrix3& lhs, const Vector3& rhs)
	{
		return Vector3(
			Dot(Vector3(lhs(0, 0), lhs(0, 1), lhs(0, 2)), rhs),
			Dot(Vector3(lhs(1, 0), lhs(1, 1), lhs(1, 2)), rhs),
			Dot(Vector3(lhs(2, 0), lhs(2, 1), lhs(2, 2)), rhs)
		);
	}

	constexpr inline Vector3 operator / (const Matrix3& lhs, const Vector3& rhs)
	{
		return lhs * Vector3(1.f / rhs.x, 1.f / rhs.y, 1.f / rhs.z);
	}
}
#pragma endregion

namespace Theater
{
	constexpr inline Matrix3 IMat3(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f));
}