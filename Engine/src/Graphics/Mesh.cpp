#include <glad/glad.h>

#include <cassert>

#include <Theater/Engine/Graphics/Mesh.hpp>

#pragma region Mesh
namespace Theater
{
	Mesh::Mesh() :
		m_VAO(0u),
		m_VBO(0u),
		m_EBO(0u),
		m_Type(MeshType::Null)
	{}

	Mesh::Mesh(VerticesVec&& vertices, IndicesVec&& indices, Material&& material) : Mesh()
	{ 
		m_Vertices = std::move(vertices);
		m_Indices = std::move(indices);
		m_Material = std::move(material);
	}

	Mesh::~Mesh()
	{
		if (m_VAO)
		{
			glDeleteBuffers(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_EBO);

			m_VAO = m_VBO = m_EBO = 0u;
		}
	}

	void Mesh::Create(MeshType type)
	{
		m_Type = type;

		glGenVertexArrays(1u, &m_VAO);
		assert(m_VAO);

		glGenBuffers(1u, &m_VBO);
		glGenBuffers(1u, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], m_Type == MeshType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], m_Type == MeshType::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0u);
		glVertexAttribPointer(0u, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1u);
		glVertexAttribPointer(1u, 2u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0u);
	}

	void Mesh::Draw(Shader& shader)
	{
		shader.SetUniform("_uShininess", m_Material.Shininess);

		glBindVertexArray(m_VAO);

		int32_t activeDiffuseTextures = 0;
		for (size_t i = 0; i < m_Material.DiffuseTextures.size(); i++)
		{
			m_Material.DiffuseTextures[i].Bind(activeDiffuseTextures);

			shader.SetUniform("_uDiffuseTextures[" + std::to_string(activeDiffuseTextures) + "]", activeDiffuseTextures);

			activeDiffuseTextures++;
		}
		shader.SetUniform("_uNumDiffuseTextures", activeDiffuseTextures);

		int32_t activeSpecularTextures = 0;
		for (size_t i = 0; i < m_Material.SpecularTextures.size(); i++)
		{
			m_Material.SpecularTextures[i].Bind(activeDiffuseTextures + activeSpecularTextures);

			shader.SetUniform("_uSpecularTextures[" + std::to_string(activeSpecularTextures) + "]", activeDiffuseTextures + activeSpecularTextures);
			activeSpecularTextures++;
		}
		
		shader.SetUniform("_uNumSpecularTextures", activeSpecularTextures);

		glActiveTexture(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}
}
#pragma endregion