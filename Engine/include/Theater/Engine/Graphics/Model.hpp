#pragma once

#include <cstdint>
#include <string_view>

#include "../EngineAPI.hpp"

#include "Mesh.hpp"

namespace Theater
{
	class ENGINE_API Model
	{
	public:

		Model() = default;
		~Model() = default;

		Model(const Model&) = delete;
		Model& operator = (const Model&) = delete;

		Model(Model&& other) noexcept
		{
			*this = std::forward<Model>(other);
		}

		Model& operator = (Model&& other) noexcept
		{
			if (this != &other)
			{
				m_Meshes = std::move(other.m_Meshes);
			}
			return *this;
		}

	public:

		void Destroy();

		bool Load(std::string_view filePath);

		void Load(Mesh&& mesh);
		void Load(std::vector<Mesh>&& meshes);

	public:

		void ClearError() { m_Error.clear(); }

		void Draw(Shader& shader);

	public:

		std::string_view GetError() const { return m_Error; }
		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		
	private:

		void Create();

		void ParseAiNode(const void* _pAiScene, void* _pAiNode); 
		void ParseAiMesh(void* _pAiMesh, const void* _pAiScene);
		void LoadMaterialTexture(const void* _pAiScene, void* _pAiMaterial, int32_t _texType, std::vector<Texture2D>& thtTextures);
		void LoadEmbeddedTexture(const void* _pAiTexture, std::vector<Texture2D>& thtTextures);
		
	private:
		std::string m_Error;
		std::vector<Mesh> m_Meshes;
	};
}