#pragma once

#include <utility>
#include <memory>

#include "../EngineAPI.hpp"
#include "../Graphics/IRenderable.hpp"
#include "../Math/Vector3.hpp"

namespace Theater
{
	struct Vertex
	{
		Vector3 Position;
		Vector2 TexCoords;
	};

	class ENGINE_API TexturedQuad
	{
	public:
		
		Vertex m_VertexData[6];

	public:

		TexturedQuad() = default;
		~TexturedQuad();

		TexturedQuad(const TexturedQuad&) = delete;
		TexturedQuad& operator = (const TexturedQuad&) = delete;
		
		TexturedQuad(TexturedQuad&& other) noexcept
		{
			*this = std::forward<TexturedQuad>(other);
		}

		TexturedQuad& operator = (TexturedQuad&& other) noexcept
		{
			if (this != &other)
			{
				m_VAO = other.m_VAO;
				m_VBO = other.m_VBO;
				std::memcpy(m_VertexData, other.m_VertexData, sizeof(m_VertexData));

				std::memset(other.m_VertexData, 0, sizeof(other.m_VertexData));
				other.m_VAO = other.m_VBO = 0;
			}
			return *this;
		}

		uint32_t GetVAO() const { return m_VAO; }
		uint32_t GetVBO() const { return m_VBO; }

	public:

		void Draw(Shader& shader, uint32_t textureID);
		void Create();

	private:
		uint32_t m_VAO{ 0 };
		uint32_t m_VBO{ 0 };
	};
}