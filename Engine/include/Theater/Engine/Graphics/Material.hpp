#pragma once

#include <vector>

#include "Texture.hpp"
#include "../Math/Vector4.hpp"

namespace Theater
{
	struct Material
	{
		float Shininess{ 0.f };
		Vector4 AmbientColor;
		Vector4 DiffuseColor;
		Vector4 SpecularColor;

		std::vector<Texture2D> DiffuseTextures;
		std::vector<Texture2D> SpecularTextures;
		std::vector<Texture2D> NormalTextures;
	};
}