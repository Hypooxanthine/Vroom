#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"

namespace vrm::gl
{

  class VRM_API OwningFrameBuffer : public FrameBuffer
  {
  public:
    inline constexpr OwningFrameBuffer()
        : FrameBuffer()
    {
    }

    inline virtual ~OwningFrameBuffer()
    {
      release();
    }

    inline virtual void release() noexcept override
    {
      if (isCreated())
      {
        FrameBuffer::release();

        for (auto &tex : m_colorTextures)
          tex.release();
        m_depthTexture.release();
        m_renderBuffer.release();
      }
    }

    // Attachment getters

    inline const Texture2D &getColorAttachmentTexture(GLuint slot) const
    {
      return m_colorTextures.at(slot);
    }

    inline const Texture2D &getDepthAttachmentTexture() const
    {
      return m_depthTexture;
    }

    // Attachment setters

    inline void setColorAttachment(GLuint slot, GLuint channels, const glm::vec4 &clearColor = glm::vec4{0.f, 0.f, 0.f, 1.f})
    {
      VRM_ASSERT_MSG(isCreated(), "Framebuffer is not created");
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Color attachment on slot {} is already used", slot);

      bool genMipMaps = (m_samples == 1);
      m_colorTextures.at(slot).createColors(m_width, m_height, channels, m_samples, nullptr, genMipMaps);
      FrameBuffer::setColorAttachment(slot, m_colorTextures.at(slot), 0, clearColor);
    }

    inline void setDepthAttachment(float depthClearValue = 1.f)
    {
      VRM_ASSERT_MSG(isCreated(), "Framebuffer is not created");
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");

      m_depthTexture.createDepth(m_width, m_height);
      FrameBuffer::setDepthAttachment(m_depthTexture, 0, depthClearValue);
    }

    inline void setRenderBufferDepthAttachment(float depthClearValue = 1.f)
    {
      VRM_ASSERT_MSG(isCreated(), "Framebuffer is not created");
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      
      m_renderBuffer.create(m_width, m_height, m_samples);

      FrameBuffer::setRenderBufferDepthAttachment(m_renderBuffer, depthClearValue);
     }

    inline void resize(GLuint width, GLuint height)
    {
      VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");

      if (m_width == width && m_height == height)
      {
        return;
      }

      bind();

      m_width = width;
      m_height = height;

      for (GLuint slot = 0; slot < m_colorTextures.size(); ++slot)
      {
        if (isColorAttachmentUsed(slot))
        {
          auto &colTex = m_colorTextures.at(slot);
          auto channels = colTex.getChannels();
          bool genMipMaps = (m_samples == 1);

          m_colorTextures.at(slot).createColors(m_width, m_height, channels, m_samples, nullptr, genMipMaps);
        }
      }

      if (isDepthAttachmentUsed())
      {
        switch (getDepthAttachmentType())
        {
          case EDepthAttachmentType::eDepthTexture:
          {
            m_depthTexture.createDepth(m_width, m_height);
            // glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture.getRendererID(), 0);
            break;
          }
          case EDepthAttachmentType::eRenderBufferDepthStencil:
          {
            m_renderBuffer.create(m_width, m_height, m_samples);
            // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer.getRenderID());
            break;
          }
          default:
          {
            VRM_ASSERT_MSG(false, "Unsupported depth attachment type");
          }
        }
      }
    }

  private:
    std::array<Texture2D, s_MaxColorAttachments> m_colorTextures;
    Texture2D m_depthTexture;
    RenderBuffer m_renderBuffer;
  };

} // namespace vrm::gl
