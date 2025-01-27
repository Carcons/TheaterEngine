#pragma once

#include <vector>

#include "../EngineAPI.hpp"

#include "Material.hpp"
#include "Vertex.hpp"
#include "IRenderable.hpp"
#include "Shader.hpp"

namespace Theater
{
	using VerticesVec = std::vector<Vertex>;
	using IndicesVec = std::vector<uint32_t>;
	using MaterialsVec = std::vector<Material>;

	class Shader;

	enum class MeshType
	{
		Null,
		Static,
		Dynamic
	};

	class ENGINE_API Mesh : public IRenderable
	{
		friend class RenderingSystem;

	public:

		Mesh();
		Mesh(VerticesVec&& vertices, IndicesVec&& indices, Material&& material);
		~Mesh();

		Mesh(Mesh&& other) noexcept
		{
			*this = std::forward<Mesh>(other);
		}

		Mesh& operator = (Mesh&& other) noexcept
		{
			if (this != &other)
			{
				m_VAO = other.m_VAO;
				m_VBO = other.m_VBO;
				m_EBO = other.m_EBO;

				m_Vertices = std::move(other.m_Vertices);
				m_Indices = std::move(other.m_Indices);
				m_Material = std::move(other.m_Material);

				other.m_VAO = other.m_VBO = other.m_EBO = 0u;
			}

			return *this;
		}

	public:

		void Create(MeshType type = MeshType::Static);

	public:

		void Draw(Shader& shader) override;
		void SetVertices(VerticesVec&& vertices) { m_Vertices = std::move(vertices); }
		void SetIndices(IndicesVec&& indices) { m_Indices = std::move(indices); }
		void SetMaterial(Material&& material) { m_Material = std::move(material); }

	public:

		MeshType GetType() const { return m_Type; }
		const VerticesVec& GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
		const Material& GetMaterial() const { return m_Material; }

		uint32_t GetVAO() const { return m_VAO; }
		uint32_t GetVBO() const { return m_VBO; }
		uint32_t GetEBO() const { return m_EBO; }

	private:

		MeshType m_Type;

		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;

		VerticesVec m_Vertices;
		IndicesVec m_Indices;
		Material m_Material;
	};
}