#pragma once

#include <array>
#include <memory>
#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "Vroom/Render/Abstraction/GLCall.h"

#include "Vroom/Render/Abstraction/Texture.h"
#include "Vroom/Render/Abstraction/RenderBuffer.h"

namespace vrm::gl
{

  class VRM_API FrameBuffer
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

  protected:
    static inline constexpr size_t s_MaxColorAttachments = 8;

  private:
    template <typename T, size_t Count>
    inline consteval static std::array<T, Count> GetFilledArray(const T& defaultVal)
    {
      std::array<T, Count> arr{};
      for (size_t i = 0; i < Count; ++i)
        arr[i] = defaultVal;
      return arr;
    }

    template <size_t N = s_MaxColorAttachments>
    inline consteval static std::array<GLenum, N> GetEmptyDrawBuffers()
    {
      return GetFilledArray<GLenum, N>(GL_NONE);
    }

    template <size_t N = s_MaxColorAttachments>
    inline consteval static std::array<glm::vec4, N> GetEmptyClearColors()
    {
      return GetFilledArray<glm::vec4, N>(glm::vec4{ 0.f, 0.f, 0.f, 1.f });
    }

  public:
    inline constexpr FrameBuffer()
    {
    }

    inline ~FrameBuffer() noexcept
    {
      release();
    }

    inline static const FrameBuffer& GetDefaultFrameBuffer();

    inline virtual void release() noexcept
    {
      if (isCreated())
      {
        glDeleteFramebuffers(1, &m_renderID);
        m_renderID = 0;
        m_width = 0;
        m_height = 0;
        m_samples = 0;

        m_anyColorAttachment = false;
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

    inline void bind() const
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
    }

    inline constexpr GLuint getRenderID() const
    {
      return m_renderID;
    }

    inline constexpr GLuint getWidth() const
    {
      return m_width;
    }

    inline constexpr GLuint getHeight() const
    {
      return m_height;
    }

    inline constexpr GLuint getSamples() const
    {
      return m_samples;
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

    inline void setColorAttachment(GLuint slot, const Texture& colorTexture, GLuint mipLevel = 0, const glm::vec4& clearColor = glm::vec4{ 0.f, 0.f, 0.f, 1.f })
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Color attachment on slot {} is already used", slot);
      VRM_ASSERT_MSG(colorTexture.getDescription().sampleCount == m_samples, "Samples must match: framebuffer is set with {} samples, but texture has {}",
        m_samples, colorTexture.getDescription().sampleCount);

      bind();
      colorTexture.bind();

      GLenum attachment = GL_COLOR_ATTACHMENT0 + slot;

      glFramebufferTexture(GL_FRAMEBUFFER, attachment, colorTexture.getRenderId(), mipLevel);
      m_drawBuffers.at(slot) = attachment;
      m_clearColors.at(slot) = clearColor;

      m_anyColorAttachment = true;
    }

    inline void setColorAttachment(GLuint slot, const Texture& arrayColorTexture, GLuint layer, GLint mipLevel = 0, const glm::vec4& clearColor = glm::vec4{ 0.f, 0.f, 0.f, 1.f })
    {
      VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Slot is too high: {}, max is {}", slot, s_MaxColorAttachments - 1);
      VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Color attachment on slot {} is already used", slot);
      const Texture::Desc& desc = arrayColorTexture.getDescription();
      VRM_ASSERT_MSG(desc.sampleCount == m_samples, "Samples must match: framebuffer is set with {} samples, but texture has {}", m_samples, desc.sampleCount);
      VRM_ASSERT_MSG(desc.depth > layer, "Texture has not enough layers");

      bind();
      arrayColorTexture.bind();

      GLenum attachment = GL_COLOR_ATTACHMENT0 + slot;

      glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, arrayColorTexture.getRenderId(), mipLevel, layer);
      m_drawBuffers.at(slot) = attachment;
      m_clearColors.at(slot) = clearColor;

      m_anyColorAttachment = true;
    }

    inline void setDepthAttachment(const Texture& depthTexture, GLuint mipLevel = 0, float depthClearValue = 1.f)
    {
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      VRM_ASSERT_MSG(depthTexture.getDescription().sampleCount == 1, "Only 1 sample for depth");

      bind();
      depthTexture.bind();

      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture.getRenderId(), mipLevel);

      m_depthAttachment = EDepthAttachmentType::eDepthTexture;
      m_depthClearValue = depthClearValue;
    }

    inline void setDepthAttachment(const Texture& arrayDepthTexture, GLuint layer, GLint mipLevel = 0, float depthClearValue = 1.f)
    {
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      const Texture::Desc& desc = arrayDepthTexture.getDescription();
      VRM_ASSERT_MSG(desc.sampleCount == 1, "Only 1 sample for depth");
      VRM_ASSERT_MSG(desc.depth > layer, "Texture has not enough layers");

      bind();
      arrayDepthTexture.bind();

      glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, arrayDepthTexture.getRenderId(), mipLevel, layer);

      m_depthAttachment = EDepthAttachmentType::eDepthTexture;
      m_depthClearValue = depthClearValue;
    }

    inline void setRenderBufferDepthAttachment(const RenderBuffer& renderBuffer, float depthClearValue = 1.f)
    {
      VRM_ASSERT_MSG(!isDepthAttachmentUsed(), "Depth attachment is already used");
      VRM_ASSERT_MSG(renderBuffer.isCreated(), "RenderBuffer not created");

      bind();

      // For now RenderBuffer class is only supporting depth + stencil.
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.getRenderID());

      m_depthAttachment = EDepthAttachmentType::eRenderBufferDepthStencil;
      m_depthClearValue = depthClearValue;
    }

    // Framebuffer operations

    inline bool validate()
    {
      bind();

      if (m_anyColorAttachment)
      {
        glDrawBuffers(s_MaxColorAttachments, m_drawBuffers.data());
      }
      else
      {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
      }

      GLenum errCode = glCheckFramebufferStatus(GL_FRAMEBUFFER);

      if (errCode == GL_FRAMEBUFFER_COMPLETE)
      {
        return true;
      }

      VRM_LOG_WARN("Framebuffer incomplete, error code: {:x}", errCode);
      return false;
    }

    inline void clearColors() const
    {
      bind();

      for (GLuint slot = 0; slot < s_MaxColorAttachments; ++slot)
      {
        if (isColorAttachmentUsed(slot))
        {
          glClearBufferfv(GL_COLOR, slot, &m_clearColors.at(slot).r);
        }
      }
    }

    inline void clearColors(const glm::vec4& customClearColor) const
    {
      bind();

      for (GLuint slot = 0; slot < s_MaxColorAttachments; ++slot)
      {
        if (isColorAttachmentUsed(slot))
        {
          glClearBufferfv(GL_COLOR, slot, &customClearColor.r);
        }
      }
    }

    inline void clearDepth() const
    {
      clearDepth(m_depthClearValue);
    }

    inline void clearDepth(float customClearDepth) const
    {
      if (isDepthAttachmentUsed())
      {
        glClearBufferfv(GL_DEPTH, 0, &customClearDepth);
      }
    }

    inline static void Blit(const FrameBuffer& dest, const FrameBuffer& src)
    {
      VRM_ASSERT_MSG(src.isCreated() && dest.isCreated(), "Src or dest framebuffer is not created");

      glBindFramebuffer(GL_READ_FRAMEBUFFER, src.getRenderID());
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.getRenderID());

      glBlitFramebuffer(
        0, 0, src.m_width, src.m_height,
        0, 0, dest.m_width, dest.m_height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

  protected:
    GLuint m_width = 0, m_height = 0;
    GLuint m_samples = 0;

  private:
    bool m_anyColorAttachment = false;
    std::array<GLenum, s_MaxColorAttachments> m_drawBuffers = GetEmptyDrawBuffers();
    std::array<glm::vec4, s_MaxColorAttachments> m_clearColors = GetEmptyClearColors();

    EDepthAttachmentType m_depthAttachment = EDepthAttachmentType::eNone;
    float m_depthClearValue = 1.f;

    GLuint m_renderID = 0;

    static std::unique_ptr<FrameBuffer> s_DefaultFrameBuffer;
  };

  inline const FrameBuffer& FrameBuffer::GetDefaultFrameBuffer()
  {
    if (s_DefaultFrameBuffer == nullptr)
    {
      s_DefaultFrameBuffer.reset(new FrameBuffer());
    }

    return *s_DefaultFrameBuffer;
  }

} // namespace vrm::gl
