#pragma once

#include <cstdint>
#include <string>

#include "../EngineAPI.hpp"
#include "../Math/Vector4.hpp"
#include "Texture.hpp"

namespace Theater
{
    class ENGINE_API RenderingTarget 
    {
        friend class RenderingSystem;

    public:

        enum class TargetType 
        {
            Screen,
            Texture
        };

        RenderingTarget();
        RenderingTarget(uint32_t width, uint32_t height, TargetType targetType = TargetType::Screen);
        ~RenderingTarget();

        RenderingTarget(const RenderingTarget&) = delete;
        RenderingTarget& operator = (const RenderingTarget&) = delete;

        RenderingTarget(RenderingTarget&& other) noexcept
        {
            *this = std::forward<RenderingTarget>(other);
        }

        RenderingTarget& operator = (RenderingTarget&& other) noexcept
        {
            if (this != &other)
            {
                m_Width = other.m_Width;
                m_Height = other.m_Height;
                m_DepthAttachment = other.m_DepthAttachment;
                m_ColorAttachment = other.m_ColorAttachment;
                m_Framebuffer = other.m_Framebuffer;
                m_FramebufferError = std::move(other.m_FramebufferError);

                other.m_Width = other.m_Height = \
                    other.m_DepthAttachment = other.m_ColorAttachment = other.m_Framebuffer = 0;
            }

            return *this;
        }

    public:

        bool Create();
        bool Create(uint32_t width, uint32_t height, TargetType targetType = TargetType::Screen);

        void ClearError() { m_FramebufferError.clear(); }
        void Resize(uint32_t newWidth, uint32_t newHeight);

        void Bind();
        void UnBind();

    public:

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        uint32_t GetFramebufferID() const { return m_Framebuffer; }
        uint32_t GetTextureID() const { return m_ColorAttachment; }

        TargetType GetTargetType() const { return m_TargetType; }

        bool HasError() const { return m_FramebufferError.size(); }
        std::string GetError() const { return m_FramebufferError; }

    private:

        void Destroy();

    private:

        std::string m_FramebufferError;

        uint32_t m_Width;
        uint32_t m_Height;

        uint32_t m_Framebuffer;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;

        TargetType m_TargetType;
    };

}