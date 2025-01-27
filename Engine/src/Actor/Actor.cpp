
#include <Theater/Engine/Graphics/Shader.hpp>
#include <Theater/Engine/Graphics/RenderingSystem.hpp>
#include <Theater/Engine/Math/Mat4.hpp>

#include <Theater/Engine/Actor/Actor.hpp>

namespace Theater
{
	Actor::Actor()
	{
		m_UUID = UUID::GenerateUUID();
	}

	Actor::~Actor()
	{}

	void Actor::OnDraw(Shader& shader)
	{
		Matrix4 modelMat = Translate(IMat4, m_Transform.Position);
		modelMat = RotateX(modelMat, m_Transform.Rotation.x);
		modelMat = RotateY(modelMat, m_Transform.Rotation.y);
		modelMat = RotateZ(modelMat, m_Transform.Rotation.z);
		modelMat = Scale(modelMat, m_Transform.Scale);

		shader.SetUniform("_uEngineModelMat", modelMat);

		m_Model.Draw(shader);
	}

	void Actor::OnUpdate(float deltaTime)
	{

	}
}