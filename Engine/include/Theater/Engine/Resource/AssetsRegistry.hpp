#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "../System.hpp"
#include "../Types.hpp"

namespace Theater
{
	enum class AssetFieldType : u16
	{
		Mesh,
		Texture,
		Audio,
		Text,
		Generic_Binary
	};

	enum class AssetType : u16
	{
		// Group of meshes asset
		Model,
		// Single texture asset
		Texture,
		// ASCII text asset
		Text,
		// Single sound asset
		Sound,
		// Generic binary asset
		Generic
	};

	struct AssetField
	{
		AssetFieldType Type;
		std::unique_ptr<void> pData;
	};

	struct AssetHeader
	{
		AssetType Type;
		u32 NumFields;
	};

	struct ModelHeader
	{
		u16 Type{ static_cast<u16>(AssetType::Model) };
		u32 NumMeshes;
		u32 NumExternalTextures;
	};

	class AssetsRegistry : public System<AssetsRegistry>
	{
	public:

		template<typename T>
		void WriteAssetOnDisk(std::string_view path, T& header, std::vector<AssetField>&& fields);
		
	private:

	};
}