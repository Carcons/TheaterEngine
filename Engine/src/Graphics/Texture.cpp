#include <glad/glad.h>

#include <cassert>
#include <Theater/Engine/Graphics/Texture.hpp>

namespace Theater
{
	Texture2D::Texture2D() :
		m_ID(0u),
		m_Width(0u),
		m_Height(0u),
		m_Channels(0u)
	{}

	Texture2D::Texture2D(const TextureCreationInfo& nfo) : Texture2D()
	{
		Create(nfo);
	}

	Texture2D::~Texture2D()
	{
		Destroy();
	}

	void Texture2D::Bind(uint32_t texUnit)
	{
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture2D::UnBind()
	{
		glActiveTexture(GL_TEXTURE0);
	}

	bool Texture2D::Create(const TextureCreationInfo& nfo)
	{
		assert(nfo.pData && "Data passed to Texture2D is nullptr");
		if (!nfo.pData) {
			return false;
		}

		glGenTextures(1u, &m_ID);
		assert(m_ID && "Can't create Texture2D");

		if (!m_ID) {
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, m_ID);

		#pragma region WrapOptions
		{
			int32_t wrapOptions = 0;
			switch (nfo.WrapOptions)
			{
			case TextureWrapping::Repeat: { wrapOptions = GL_REPEAT; break; }
			case TextureWrapping::MirroredRepeat: { wrapOptions = GL_MIRRORED_REPEAT; break; }
			case TextureWrapping::ClampToEdge: { wrapOptions = GL_CLAMP_TO_EDGE; break; }
			case TextureWrapping::ClampToBorder: { wrapOptions = GL_CLAMP_TO_BORDER; break; }
			default: wrapOptions = GL_REPEAT;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOptions);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOptions);
		}
		#pragma endregion

		#pragma region Mipmap Filter
		{
			if (nfo.GenerateMipmap)
			{
				int32_t filter = 0;
				switch (nfo.MipmapFilter)
				{
				case TextureMipmapFilter::NearestMipmapNearest: { filter = GL_NEAREST_MIPMAP_NEAREST; break; }
				case TextureMipmapFilter::LinearMipmapNearest: { filter = GL_LINEAR_MIPMAP_NEAREST; break; }
				case TextureMipmapFilter::NearestMipmapLinear: { filter = GL_NEAREST_MIPMAP_LINEAR; break; }
				case TextureMipmapFilter::LinearMipmapLinear: { filter = GL_LINEAR_MIPMAP_LINEAR; break; }
				default: filter = GL_LINEAR_MIPMAP_LINEAR;
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

			}
			else
			{
				const int32_t Filter = nfo.OnMinFilter == TextureFiltering::Nearest ? GL_NEAREST : GL_LINEAR;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
			}
		}
		#pragma endregion

		#pragma region Texture data and Mipmap Generation
		{
			const int32_t MagFilter = nfo.OnMagFilter == TextureFiltering::Nearest ? GL_NEAREST : GL_LINEAR;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);

			int32_t channels = 0;
			if (nfo.Channels == 3u)
				channels = GL_RGB;
			else
				channels = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0u, channels, nfo.Width, nfo.Height, 0u, channels, GL_UNSIGNED_BYTE, nfo.pData);

			if (nfo.GenerateMipmap)
				glGenerateMipmap(GL_TEXTURE_2D);
		}
		#pragma endregion

		m_Width = nfo.Width;
		m_Height = nfo.Height;
		m_Channels = nfo.Channels;

		glBindTexture(GL_TEXTURE_2D, 0u);

		return true;

	}

	//bool Texture2D::CreateForRendering(int32_t w, int32_t h, uint32_t samples)
	//{
	//	glGenTextures(1, &m_ID);
	//	if (!m_ID) 
	//		return false;

	//	glBindTexture(GL_TEXTURE_2D, m_ID);

	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	return true;
	//}

	void Texture2D::Destroy()
	{
		if (m_ID)
		{
			glDeleteTextures(1u, &m_ID);
			m_ID = 0u;
		}
	}

	uint32_t Texture2D::Release()
	{
		const uint32_t ID = m_ID;
		m_ID = 0u;

		return ID;
	}
}