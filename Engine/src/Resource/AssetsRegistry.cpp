//#include <fstream>
//#include <filesystem>
//#include <Theater/Engine/AssetsRegistry/AssetsRegistry.hpp>
//
//namespace stdfs = std::filesystem;
//
//namespace Theater
//{
//	bool WriteAssetOnDisk(std::string_view physicalPath, AssetHeader& header, std::vector<AssetField>&& assetFields)
//	{
//		const stdfs::path cAssetPath = physicalPath;
//		if (!stdfs::exists(cAssetPath))
//			return false;
//
//		std::ofstream writer(cAssetPath, std::ios::binary | std::ios::out);
//		if (writer.is_open())
//		{
//			writer.write(reinterpret_cast<char*>(&header), sizeof(header));
//			
//			for (const auto& field : assetFields)
//			{
//
//			}
//
//			return true;
//		}
//
//		return false;
//	}
//
//	AssetPtr AssetsRegistry::LoadAsset(std::string_view path)
//	{
//		// Check if already in memory
//		auto it = m_Assets.find(std::string(path));
//		if (it != m_Assets.end())
//		{
//			return &(it->second);
//		}
//
//		// Load from disk
//		{
//			Asset asset;
//
//			const stdfs::path cAssetPath = stdfs::path(path);
//			if (!stdfs::exists(cAssetPath))
//			{
//				return nullptr;
//			}
//
//			std::ifstream reader(cAssetPath);
//			
//			if (cAssetPath.extension().compare("thtasset"))
//			{
//				// Editor asset, extra field (texture) preview
//				
//			}
//		}
//
//		return nullptr;
//	}
//}