#pragma once

#include <utility> //std::forward
#include <cstdint>

#include "../EngineAPI.hpp"
#include "../Resource/Resource.hpp"

namespace Theater
{
	enum class TextureWrapping
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFiltering
	{
		Nearest,
		Linear
	};

	enum class TextureMipmapFilter
	{
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	struct TextureCreationInfo
	{
		const uint8_t* pData{ nullptr };
		uint32_t Width{ 0u };
		uint32_t Height{ 0u };
		uint16_t Channels{ 0u };
		bool GenerateMipmap{ true };
		TextureWrapping WrapOptions{ TextureWrapping::Repeat };
		TextureFiltering OnMagFilter{ TextureFiltering::Linear };
		TextureFiltering OnMinFilter{ TextureFiltering::Nearest };
		TextureMipmapFilter MipmapFilter{ TextureMipmapFilter::LinearMipmapLinear };
	};

	class ENGINE_API Texture2D
	{
	public:

		Texture2D();
		explicit Texture2D(const TextureCreationInfo& nfo);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator = (const Texture2D&) = delete;

		Texture2D(Texture2D&& other) noexcept
		{
			*this = std::forward<Texture2D>(other);
		}

		Texture2D& operator = (Texture2D&& other) noexcept
		{
			if (this != &other)
			{
				m_ID = other.m_ID;
				m_Width = other.m_Width;
				m_Height = other.m_Height;
				m_Channels = other.m_Channels;

				other.m_ID = other.m_Width = other.m_Height = other.m_Channels = 0u;
			}

			return *this;
		}

	public:

		void Bind(uint32_t texUnit = 0u);
		void UnBind();

		bool Create(const TextureCreationInfo& nfo);

		void Destroy();

	public:

		uint32_t Release();

		uint32_t GetID() const { return m_ID; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint16_t GetChannels() const { return m_Channels; }

	private:

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ID;
		uint16_t m_Channels;
	};
}