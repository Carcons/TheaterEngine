#pragma once

#include "../Math/Vector4.hpp"

namespace Theater
{
	struct Transform 
	{
		Vector3 Position;
		Vector3 Scale{ 1.f, 1.f, 1.f };
		Vector3 Rotation;
	};
}