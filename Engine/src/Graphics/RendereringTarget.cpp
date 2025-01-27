#include <glad/glad.h>

#include <iostream>
#include <cassert>

#include <Theater/Engine/Graphics/RenderingTarget.hpp>

namespace Theater
{
	RenderingTarget::RenderingTarget() :
		m_Width(0),
		m_Height(0),
		m_TargetType(TargetType::Screen),
		m_DepthAttachment(0),
		m_ColorAttachment(0),
		m_Framebuffer(0)
	{}

	RenderingTarget::RenderingTarget(uint32_t width, uint32_t height, TargetType targetType) :
		m_Width(width),
		m_Height(height),
		m_TargetType(targetType),
		m_DepthAttachment(0),
		m_ColorAttachment(0),
		m_Framebuffer(0)
	{}

	RenderingTarget::~RenderingTarget()
	{
		Destroy();
	}

	void RenderingTarget::Resize(uint32_t newWidth, uint32_t newHeight)
	{
        m_Width = newWidth;
        m_Height = newHeight;

		Create();
	}

	void RenderingTarget::Bind()
	{ 
		if (m_TargetType == TargetType::Screen) 
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        else 
            glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	}

	void RenderingTarget::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingTarget::Destroy()
	{
		if (m_Framebuffer)
		{
			glDeleteFramebuffers(1, &m_Framebuffer);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteRenderbuffers(1, &m_DepthAttachment);
		}
	}

	bool RenderingTarget::Create()
	{
		if (m_TargetType == TargetType::Screen)
		{
			return false;
		}

		Destroy();
		
        glGenFramebuffers(1, &m_Framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

		glGenTextures(1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glGenRenderbuffers(1, &m_DepthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

		const GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			m_FramebufferError = "Framebuffer error: ";
			switch (Status)
			{
			case GL_FRAMEBUFFER_UNDEFINED:
				m_FramebufferError += "GL_FRAMEBUFFER_UNDEFINED";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				m_FramebufferError += "GL_FRAMEBUFFER_UNSUPPORTED";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" ;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				m_FramebufferError += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" ;
				break;
			default:
				m_FramebufferError += "Unknown error";
				break;
			}

			Destroy();

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	bool RenderingTarget::Create(uint32_t width, uint32_t height, TargetType targetType)
	{
		m_Width = width;
		m_Height = height;
		m_TargetType = targetType;
		m_DepthAttachment = 0;
		m_ColorAttachment = 0;
		m_Framebuffer = 0;

		return Create();
	}
}