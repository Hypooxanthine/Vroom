#pragma once

#include <array>

#include "Vroom/Render/Abstraction/GLCall.h"

#include "Vroom/Render/Abstraction/Texture2D.h"
#include "Vroom/Render/Abstraction/ArrayTexture2D.h"

namespace vrm::gl
{
  
  class RawFrameBuffer
  {
  public:

    enum EDepthAttachmentType : uint8_t
    {
      eNone = 0,
      eDepthTexture,
      eDepthStencilTexture,
      eRenderBufferDepth,
      eRenderBufferDepthStencil
    };

  public:
    inline constexpr RawFrameBuffer()
    {
    }

    inline ~RawFrameBuffer() noexcept
    {
      release();
    }

    inline void release() noexcept
    {
      if (isCreated())
      {
        glDeleteFramebuffers(1, &m_renderID);
        m_renderID = 0;
        m_width = 0;
        m_height = 0;
        m_samples = 0;

        m_drawBuffers = GetEmptyDrawBuffers();
        m_depthAttachment = EDepthAttachmentType::eNone;
      }
    }

    inline constexpr bool isCreated() const
    {
      return m_renderID != 0;
    }

    inline void create(GLuint width, GLuint height, GLuint samples)
    {
      release();

      glGenFramebuffers(1, &m_renderID);

      m_width = width;
      m_height = height;
      m_samples = samples;
    }

    inline void bind()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
    }

    // Attachment infos

    inline constexpr bool isColorAttachmentUsed(GLuint slot) const
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      return m_drawBuffers.at(slot) != GL_NONE;
    }

    inline constexpr EDepthAttachmentType getDepthAttachmentType() const
    {
      return m_depthAttachment;
    }

    inline constexpr bool isDepthAttachmentUsed() const
    {
      return m_depthAttachment != EDepthAttachmentType::eNone;
    }

    // Attachment setters

    inline void setColorAttachment(GLuint slot, const Texture2D& colorTexture, GLuint mipLevel = 0)
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Color attachment on slot {} is already used", slot);
      VRM_ASSERT_MSG(colorTexture.getSamples() == m_samples, "Samples must match: framebuffer is set with {} samples, but texture has {}", m_samples, colorTexture.getSamples());
      colorTexture.bind();

      GLenum attachment = GL_COLOR_ATTACHMENT0 + slot;

      glFramebufferTexture(GL_FRAMEBUFFER, attachment, colorTexture.getRendererID(), mipLevel);
      m_drawBuffers.at(slot) = attachment;
    }

    inline void setColorAttachment(GLuint slot, const ArrayTexture2D& arrayColorTexture, GLuint layer, GLint mipLevel = 0)
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Color attachment on slot {} is already used", slot);
      VRM_ASSERT_MSG(arrayColorTexture.getSamples() == m_samples, "Samples must match: framebuffer is set with {} samples, but texture has {}", m_samples, arrayColorTexture.getSamples());
      VRM_ASSERT_MSG(arrayColorTexture.getLayerCount() > layer, "ArrayTexture2D size is too small");
      arrayColorTexture.bind();

      GLenum attachment = GL_COLOR_ATTACHMENT0 + slot;

      glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, arrayColorTexture.getRenderID(), mipLevel, layer);
      m_drawBuffers.at(slot) = attachment;
    }

    inline void setDepthAttachment(GLuint slot, const Texture2D& depthTexture, GLuint mipLevel = 0)
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      VRM_ASSERT_MSG(depthTexture.getSamples() == 1, "Only 1 sample for depth");
      depthTexture.bind();
      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture.getRendererID(), mipLevel);
    }

    inline void setDepthAttachment(GLuint slot, const ArrayTexture2D& arrayDepthTexture, GLuint layer, GLint mipLevel = 0)
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      VRM_ASSERT_MSG(arrayDepthTexture.getSamples() == 1, "Only 1 sample for depth");
      VRM_ASSERT_MSG(arrayDepthTexture.getLayerCount() > layer, "ArrayTexture2D size is too small");
      
      arrayDepthTexture.bind();
      glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, arrayDepthTexture.getRenderID(), mipLevel, layer);
    }

  private:

    static inline constexpr size_t s_MaxColorAttachments = 8;

    template <size_t N = s_MaxColorAttachments>
    inline consteval static std::array<GLenum, N> GetEmptyDrawBuffers()
    {
      std::array<GLenum, N> arr{};
      for (size_t i = 0; i < N; ++i)
          arr[i] = GL_NONE;
      return arr;
    }

  private:
    std::array<GLenum, s_MaxColorAttachments> m_drawBuffers = GetEmptyDrawBuffers();
    EDepthAttachmentType m_depthAttachment = EDepthAttachmentType::eNone;

    GLuint m_renderID = 0;

    GLuint m_width = 0, m_height = 0;
    GLuint m_samples = 0;
  };

} // namespace vrm::gl
