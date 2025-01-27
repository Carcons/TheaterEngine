#pragma once

#pragma region Debug helpers

#if defined(DEBUG) || defined(_DEBUG)

#include <iostream>
#include <iomanip>

#include "Mat3.hpp"
#include "Mat4.hpp"

namespace Theater
{
	namespace Detail
	{
		inline void _DebugMatrix(const char* matName, const Matrix3& mat)
		{
			std::cout << matName << ":\n";
			for (int i = 0; i < 3; i++)
			{
				std::cout << "| ";

				for (int j = 0; j < 3; j++)
					std::cout << std::setw(9) << std::fixed << std::setprecision(4) << mat(i, j) << " ";

				std::cout << "|\n";
			}
			std::cout << "------------------------------------------------\n";
		}

		inline void _DebugMatrix(const char* matName, const Matrix4& mat)
		{
			std::cout << matName << ":\n";
			for (int i = 0; i < 4; i++)
			{
				std::cout << "| ";

				for (int j = 0; j < 4; j++)
					std::cout << std::setw(9) << std::fixed << std::setprecision(4) << mat(i, j) << " ";

				std::cout << "|\n";
			}
			std::cout << "------------------------------------------------\n";
		}

		inline void _DebugVector(const char* vecName, const Vector2& vec)
		{
			std::cout << vecName << ": (" << vec.x << "," << vec.y << ")\n";
			std::cout << "------------------------------------------------\n";
		}

		inline void _DebugVector(const char* vecName, const Vector3& vec)
		{
			std::cout << vecName << ": (" << vec.x << "," << vec.y << "," << vec.z << ")\n";
			std::cout << "------------------------------------------------\n";
		}

		inline void _DebugVector(const char* vecName, const Vector4& vec)
		{
			std::cout << vecName << ": (" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")\n";
			std::cout << "------------------------------------------------\n";
		}
	}
}

#define DebugMatrix(Matrix) Theater::Detail::_DebugMatrix(#Matrix, Matrix)
#define DebugVector(Vector) Theater::Detail::_DebugVector(#Vector, Vector)

#else

#define DebugMatrix
#define DebugVector

#endif //_DEBUG MATH

#pragma endregion