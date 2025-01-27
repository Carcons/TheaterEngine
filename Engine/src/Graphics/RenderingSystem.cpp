#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <Theater/Engine/Math/Vector4.hpp>
#include <Theater/Engine/Graphics/Shader.hpp>
#include <Theater/Engine/Graphics/RenderingTarget.hpp>
#include <Theater/Engine/Graphics/RenderingSystem.hpp>

#include "EngineShaders.hpp"

namespace Theater
{
	RenderingSystem::RenderingSystem()
	{
		{
			const int32_t GL_Init = ::gladLoadGL();
			assert(GL_Init && "Failed to load OpenGL");
		}

		BuildEngineShaders();
	}

	RenderingSystem::~RenderingSystem()
	{
		if (m_pMeshShader)
		{
			delete m_pMeshShader;
			m_pMeshShader = nullptr;
		}
	}

	void RenderingSystem::BuildEngineShaders()
	{
		m_pMeshShader = new Shader;
		m_pMeshShader->Compile(ShadersSource::BASIC_MESH_VSHADER, ShadersSource::BASIC_MESH_FSHADER);

		m_pTexturedQuadShader = new Shader;
		m_pTexturedQuadShader->Compile(ShadersSource::TEXTUREDQUAD_VSHADER, ShadersSource::TEXTUREDQUAD_FSHADER);

		assert(m_pMeshShader->IsCompiled());
		assert(m_pTexturedQuadShader->IsCompiled());

		std::cout << "RenderingSystem: Shaders compiled\n";
	}

	void RenderingSystem::AttachTarget(RenderingTarget* pTarget)
	{
		if (pTarget)
		{
			m_pCurrentTarget = pTarget;
			m_pCurrentTarget->Bind();
		}
	}

	void RenderingSystem::DetachCurrentTarget()
	{
		if (m_pCurrentTarget)
			m_pCurrentTarget->UnBind();

		m_pCurrentTarget = nullptr;
	}

	void RenderingSystem::SetDepth(DepthFn depthFn)
	{
		if (depthFn == DepthFn::Disabled)
		{
			glDisable(GL_DEPTH_TEST);
			return;
		}

		glEnable(GL_DEPTH_TEST);

		switch (depthFn)
		{
		case DepthFn::Always:
			glDepthFunc(GL_ALWAYS);
			break;
		case DepthFn::Never:
			glDepthFunc(GL_NEVER);
			break;
		case DepthFn::Less:
			glDepthFunc(GL_LESS);
			break;
		case DepthFn::Equal:
			glDepthFunc(GL_EQUAL);
			break;
		case DepthFn::LEqual:
			glDepthFunc(GL_LEQUAL);
			break;
		case DepthFn::Greater:
			glDepthFunc(GL_GREATER);
			break;
		case DepthFn::NotEqual:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case DepthFn::GEqual:
			glDepthFunc(GL_GEQUAL);
			break;
		default:
			break;
		}
	}

	void RenderingSystem::SetBlendMode(BlendMode blendMode)
	{
		if (blendMode == BlendMode::None)
		{
			glDisable(GL_BLEND);
			return;
		}

		glEnable(GL_BLEND);

		switch (blendMode) {
		case BlendMode::Transparency:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case BlendMode::Additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case BlendMode::Multiplicative:
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case BlendMode::Subtractive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_SUBTRACT);
			break;
		case BlendMode::BurnIn:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_MAX);
			break;
		default:
			glDisable(GL_BLEND);
			break;
		}
	}

	void RenderingSystem::SetFaceCulling(FaceCullingMode mode, WindingOrder windingOrder)
	{
		if (mode == FaceCullingMode::None)
		{
			glDisable(GL_CULL_FACE);
			return;
		}

		glEnable(GL_CULL_FACE);

		switch (mode) 
		{
		case FaceCullingMode::Back:
			glCullFace(GL_BACK);
			break;
		case FaceCullingMode::Front:
			glCullFace(GL_FRONT);
			break;
		case FaceCullingMode::FrontAndBack:
			glCullFace(GL_FRONT_AND_BACK);
			break;
		default:
			break;
		}

		switch (windingOrder) 
		{
		case WindingOrder::CCW:
			glFrontFace(GL_CCW);
			break;
		case WindingOrder::CW:
			glFrontFace(GL_CW);
			break;
		default:
			break;
		}
	}

	void RenderingSystem::SetMSAA(bool enabled)
	{
		if (enabled)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}

	void RenderingSystem::SetViewport(Vector4 viewport)
	{
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	}

	void RenderingSystem::ClearColorBuffer(const Vector4& clearColor)
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderingSystem::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void RenderingSystem::ClearStencilBuffer()
	{
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void RenderingSystem::ClearAllBuffers(const Vector4& clearColor)
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	std::string RenderingSystem::GetGLVersion() const
	{
		return std::string(std::to_string(GetGLMajor()) + "." + std::to_string(GetGLMinor()));
	}

	uint32_t RenderingSystem::GetGLMajor() const
	{
		int32_t ver = 0;

		glGetIntegerv(GL_MAJOR_VERSION, &ver);
		return (uint32_t)ver;
	}

	uint32_t RenderingSystem::GetGLMinor() const
	{
		int32_t ver = 0;

		glGetIntegerv(GL_MINOR_VERSION, &ver);
		return (uint32_t)ver;
	}
}