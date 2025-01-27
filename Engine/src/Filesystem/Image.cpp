#include <stb_image.h>
#include <Theater/Engine/Filesystem/Image.hpp>

namespace Theater
{
	Image::Image() :
		m_Width(0u),
		m_Height(0u),
		m_Channels(0u)
	{}

	Image::Image(std::string_view path) : Image()
	{
		LoadFromPath(path);
	}

	bool Image::LoadFromPath(std::string_view path)
	{
		m_ErrorMsg.clear();
		m_Path = path;

		// Already loaded image
		if (m_pPixels)
		{
			m_pPixels.reset();
		}

		int32_t width = 0;
		int32_t height = 0;
		int32_t channels = 0;

		m_pPixels = std::unique_ptr<uint8_t, std::function<void(void*)>>(
			::stbi_load(path.data(), &width, &height, &channels, STBI_default),
			[](void* pPixels) { ::stbi_image_free(pPixels); }
		);

		if (!m_pPixels)
		{
			m_ErrorMsg = ::stbi_failure_reason();
			return false;
		}

		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;
		m_Channels = (uint16_t)channels;

		return true;
	}

	bool Image::LoadFromMemory(uint8_t* ptr, uint32_t length)
	{
		m_ErrorMsg.clear();

		if (m_pPixels)
			m_pPixels.reset();

		int32_t width, height, channels;

		m_pPixels = std::unique_ptr<uint8_t, std::function<void(void*)>>(
			stbi_load_from_memory(ptr, length, &width, &height, &channels, STBI_default),
			[](void* pPixels) { ::stbi_image_free(pPixels); }
		);

		if (!m_pPixels)
		{
			m_ErrorMsg = ::stbi_failure_reason();
			return false;
		}

		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;
		m_Channels = (uint16_t)channels;

		return true;
	}
}