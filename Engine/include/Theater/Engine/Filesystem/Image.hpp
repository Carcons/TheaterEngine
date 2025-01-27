#pragma once

#include <vector>
#include <functional>

#include "../EngineAPI.hpp"
#include "../Utility/String.hpp"

namespace Theater
{
	class ENGINE_API Image
	{
	public:

		Image();
		Image(std::string_view path);

		Image(const Image&) = delete;
		Image& operator = (const Image&) = delete;

		Image(Image&& other) noexcept
		{
			*this = std::forward<Image>(other);
		}

		Image& operator = (Image&& other) noexcept
		{
			if (this != &other)
			{
				m_pPixels = std::move(other.m_pPixels);
				m_Path = std::move(other.m_Path);
				m_Width = other.m_Width;
				m_Height = other.m_Height;
				m_Channels = other.m_Channels;

				other.m_Width = other.m_Height = other.m_Channels = 0u;
			}

			return *this;
		}

	public:

		bool LoadFromPath(std::string_view path);
		bool LoadFromMemory(uint8_t* ptr, uint32_t length);

	public:

		uint16_t GetChannels() const { return m_Channels; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		std::string_view GetLoadedPath() const { return m_Path.c_str(); }
		std::string_view GetLastError() const { return m_ErrorMsg; }

		const uint8_t* GetPixels() const { return m_pPixels.get(); }

	private:

		std::string m_Path;
		std::string m_ErrorMsg;

		uint16_t m_Channels;
		uint32_t m_Width;
		uint32_t m_Height;

		std::unique_ptr<uint8_t, std::function<void(void*)>> m_pPixels;
		
	};
}