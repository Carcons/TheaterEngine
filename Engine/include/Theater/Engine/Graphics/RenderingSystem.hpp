#pragma once

#include <string>

#include "../EngineAPI.hpp"
#include "../System.hpp"

#include "../Math/Vector4.hpp"

namespace Theater
{
	class Shader;
	class RenderingTarget;
	class Vector4;

	enum class DepthFn
	{
		Disabled,
		Always,
		Never,
		Less,
		Equal,
		LEqual,
		Greater,
		NotEqual,
		GEqual
	};

	enum class BlendMode 
	{
		None,
		Transparency,
		Additive,
		Multiplicative,
		Subtractive,
		BurnIn
	};

	enum class FaceCullingMode
	{
		None,         
		Back,        
		Front,      
		FrontAndBack
	};

	enum class WindingOrder 
	{
		CCW,
		CW
	};

	class ENGINE_API RenderingSystem : public System<RenderingSystem>
	{		
	public:

		RenderingSystem();
		~RenderingSystem();

	public:

		void AttachTarget(RenderingTarget* pTarget);
		void DetachCurrentTarget();

		void SetDepth(DepthFn depthFn = DepthFn::LEqual);
		void SetBlendMode(BlendMode blendMode);
		void SetFaceCulling(FaceCullingMode mode, WindingOrder windingOrder = WindingOrder::CCW);

		void SetMSAA(bool enabled);

		void SetViewport(Vector4 viewport);
		void ClearColorBuffer(const Vector4& clearColor = Vector4());
		void ClearDepthBuffer();
		void ClearStencilBuffer();
		void ClearAllBuffers(const Vector4& clearColor = Vector4());

	public:

		RenderingTarget* GetCurrentTarget() const { return m_pCurrentTarget; }

		std::string GetGLVersion() const;
		uint32_t GetGLMajor() const;
		uint32_t GetGLMinor() const;

		Shader* GetMeshShader() const { return m_pMeshShader; }
		Shader* GetTexturedQuadShader() const { return m_pTexturedQuadShader; }

	private:

		void BuildEngineShaders();

	private:

		RenderingTarget* m_pCurrentTarget = nullptr;

		Shader* m_pMeshShader;
		Shader* m_pTexturedQuadShader;
	};
}