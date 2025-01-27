#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include <Theater/Engine/Filesystem/Image.hpp>
#include <Theater/Engine/Filesystem/Image.hpp>

#include <Theater/Engine/Graphics/Model.hpp>
#include <Theater/Engine/Graphics/Animation.hpp>
#include <Theater/Engine/Graphics/Model.hpp>

namespace Theater
{
	
	void Model::Destroy()
	{
		m_Meshes.clear();
	}

	bool Model::Load(std::string_view filePath)
	{
		Destroy();

		Assimp::Importer importer;
		const uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |
			aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality |
			aiProcess_FixInfacingNormals | aiProcess_GenUVCoords |
			aiProcess_FlipUVs;

		const aiScene* pScene = importer.ReadFile(filePath.data(), flags);

		if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
		{
			m_Error = std::string("Assimp Error: ") + importer.GetErrorString();
			return false;
		}

		ParseAiNode(pScene, pScene->mRootNode);
		return true;
	}

	void Model::Load(Mesh&& mesh)
	{
		Destroy();

		m_Meshes.push_back(std::forward<Mesh>(mesh));
		Create();
	}

	void Model::Load(std::vector<Mesh>&& meshes)
	{
		Destroy();

		m_Meshes = std::forward<std::vector<Mesh>>(meshes);
		Create();
	}

	void Model::Draw(Shader& shader)
	{
		for (Mesh& mesh : m_Meshes)
		{
			mesh.Draw(shader);
		}
	}

	void Model::Create()
	{
		for (Mesh& mesh : m_Meshes)
			mesh.Create();
	}

	void Model::ParseAiNode(const void* _pAiScene, void* _pAiNode)
	{
		const aiScene* pAiScene = reinterpret_cast<const aiScene*>(_pAiScene);
		const aiNode* pAiNode = reinterpret_cast<aiNode*>(_pAiNode);

		for (size_t i = 0; i < pAiNode->mNumMeshes; i++)
		{
			ParseAiMesh(pAiScene->mMeshes[pAiNode->mMeshes[i]], pAiScene);
		}

		for (size_t i = 0; i < pAiNode->mNumChildren; i++)
		{
			ParseAiNode(pAiScene, pAiNode->mChildren[i]);
		}
	}

	void Model::ParseAiMesh(void* _pAiMesh, const void* _pAiScene)
	{
		aiMesh* pAiMesh = reinterpret_cast<aiMesh*>(_pAiMesh);
		const aiScene* pAiScene = reinterpret_cast<const aiScene*>(_pAiScene);

		VerticesVec vertices{};
		IndicesVec indices{};
		Material material{};

		// Vertices
		for (size_t i = 0; i < pAiMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = Vector3(pAiMesh->mVertices[i].x, pAiMesh->mVertices[i].y, pAiMesh->mVertices[i].z);
			vertex.TexCoords.x = pAiMesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = pAiMesh->mTextureCoords[0][i].y;

			vertices.push_back(vertex);
		}

		// Indices
		for (size_t i = 0; i < pAiMesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < pAiMesh->mFaces[i].mNumIndices; j++)
			{
				indices.push_back(pAiMesh->mFaces[i].mIndices[j]);
			}
		}

		// Material & Textures
		if (pAiMesh->mMaterialIndex >= 0)
		{
			aiMaterial* const pAiMaterial = pAiScene->mMaterials[pAiMesh->mMaterialIndex];

			float shininess = 0.f;
			if (pAiMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			{
				material.Shininess = shininess;
			}
			else
			{
				material.Shininess = 0.5f;
			}
			std::cout << "Shininess: " << material.Shininess;

			LoadMaterialTexture(pAiScene, pAiMaterial, aiTextureType_DIFFUSE, material.DiffuseTextures);
			LoadMaterialTexture(pAiScene, pAiMaterial, aiTextureType_SPECULAR, material.SpecularTextures);
		}

		m_Meshes.emplace_back(std::move(vertices), std::move(indices), std::move(material));
		m_Meshes.back().Create();
	}

	void Model::LoadMaterialTexture(const void* _pAiScene, void* _pAiMaterial, int32_t _texType, std::vector<Texture2D>& thtTextures)
	{
		const aiScene* pAiScene = reinterpret_cast<const aiScene*>(_pAiScene);
		aiMaterial* pAiMaterial = reinterpret_cast<aiMaterial*>(_pAiMaterial);

		aiTextureType texType = static_cast<aiTextureType>(_texType);
		for (size_t i = 0; i < pAiMaterial->GetTextureCount(texType); i++)
		{
			aiString str;
			pAiMaterial->GetTexture(texType, i, &str);

			if (const aiTexture* pAiTexture = pAiScene->GetEmbeddedTexture(str.C_Str()))
			{
				LoadEmbeddedTexture(pAiTexture, thtTextures);
			}
		}
	}

	void Model::LoadEmbeddedTexture(const void* _pAiTexture, std::vector<Texture2D>& thtTextures)
	{
		const aiTexture* pAiTexture = reinterpret_cast<const aiTexture*>(_pAiTexture);
		Theater::Image img;

		if (pAiTexture->mHeight == 0)
		{
			if (!img.LoadFromMemory(reinterpret_cast<uint8_t*>(pAiTexture->pcData), pAiTexture->mWidth))
				return;

			Texture2D thtTexture;
			TextureCreationInfo nfo;
			nfo.Width = img.GetWidth();
			nfo.Height = img.GetHeight();
			nfo.Channels = img.GetChannels();
			nfo.pData = img.GetPixels();

			if (thtTexture.Create(nfo))
			{
				thtTextures.emplace_back(std::move(thtTexture));
			}
		}
	}
}