#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Texture2D.h"
#include "Vroom/Render/Abstraction/RenderBuffer.h"

namespace vrm::gl
{

  class FrameBuffer
  {
  public:
    inline constexpr FrameBuffer();

    inline ~FrameBuffer();

    FrameBuffer(const FrameBuffer &other) = delete;
    FrameBuffer &operator=(const FrameBuffer &other) = delete;

    FrameBuffer(FrameBuffer &&other) = delete;
    FrameBuffer &operator=(FrameBuffer &&other) = delete;

    inline static const FrameBuffer &GetDefaultFrameBuffer();

    inline static void Blit(const FrameBuffer& dest, const FrameBuffer& src);

    inline constexpr GLuint getRenderID() const;

    inline constexpr bool isCreated() const;

    inline void bind() const;

    inline void create(GLuint width, GLuint height, GLuint MSAA = 1);

    inline void release();

    inline constexpr bool isColorAttachmentUsed(GLuint slot) const;

    inline void addColorAttachment(GLuint slot, GLuint channels = 4, const glm::vec4 &clearColor = {0.f, 0.f, 0.f, 1.f});

    inline const Texture2D &getColorAttachmentTexture(GLuint slot) const;

    inline constexpr bool isDepthAttachmentUsed() const;

    inline void addDepthAttachment(float clearValue = 1.f);

    inline const Texture2D &getDepthAttachmentTexture() const;

    inline constexpr bool isRenderBufferAttached() const;

    inline void attachRenderBuffer(float clearValue = 1.f);

    inline bool validate();

    inline bool resize(GLuint width, GLuint height);

    inline void clearColors() const;

    inline void clearColors(const glm::vec4 &customClearColor) const;

    inline void clearDepth() const;

    inline void clearDepth(float customClearDepth) const;

  private:
  private:
    static inline constexpr size_t s_MaxColorAttachments = 8;

    GLuint m_renderID = 0;
    GLuint m_width = 0, m_height = 0;
    GLuint m_samples = 0;

    std::array<Texture2D, s_MaxColorAttachments> m_colorTextures;
    std::array<glm::vec4, s_MaxColorAttachments> m_clearColors = {glm::vec4(0.f)};

    Texture2D m_depthTexture;
    RenderBuffer m_renderBuffer;
    float m_depthClearValue = 1.f;

    bool m_isDefault = false;
    inline static std::unique_ptr<FrameBuffer> s_DefaultFrameBuffer;
  };

  inline constexpr FrameBuffer::FrameBuffer()
  {
  }

  inline FrameBuffer::~FrameBuffer()
  {
    if (m_renderID != 0)
    {
      glDeleteFramebuffers(1, &m_renderID);
    }
  }

  inline const FrameBuffer &FrameBuffer::GetDefaultFrameBuffer()
  {
    if (s_DefaultFrameBuffer == nullptr)
    {
      s_DefaultFrameBuffer.reset(new FrameBuffer());
      s_DefaultFrameBuffer->m_isDefault = true;
    }

    return *s_DefaultFrameBuffer;
  }

  inline void FrameBuffer::Blit(const FrameBuffer& dest, const FrameBuffer& src)
  {
    VRM_ASSERT_MSG(src.isCreated() && dest.isCreated(), "Src or dest framebuffer is not created");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, src.getRenderID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.getRenderID());

    glBlitFramebuffer
    (
      0, 0, src.m_width, src.m_height,
      0, 0, dest.m_width, dest.m_height,
      GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
  }

  inline constexpr GLuint FrameBuffer::getRenderID() const { return m_renderID; }

  inline constexpr bool FrameBuffer::isCreated() const { return m_renderID != 0; }

  inline void FrameBuffer::bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
  }

  inline void FrameBuffer::create(GLuint width, GLuint height, GLuint MSAA)
  {
    VRM_ASSERT_MSG(MSAA > 0, "You must specify > 0 samples");

    release();

    glGenFramebuffers(1, &m_renderID);

    m_width = width;
    m_height = height;
    m_samples = MSAA;

    bind();
  }

  inline void FrameBuffer::release()
  {
    if (!isCreated())
    {
      return;
    }

    m_width = 0;
    m_height = 0;
    m_samples = 0;

    for (auto& t : m_colorTextures)
    {
      t.release();
    }

    for (auto& c : m_clearColors)
    {
      c = glm::vec4 {0.f};
    }

    m_depthTexture.release();
    m_renderBuffer.release();
    m_depthClearValue = 1.f;
    m_isDefault = false;

    glDeleteFramebuffers(1, &m_renderID);
    m_renderID = 0;
  }

  inline constexpr bool FrameBuffer::isColorAttachmentUsed(GLuint slot) const
  {
    return m_colorTextures.at(slot).isCreated();
  }

  inline void FrameBuffer::addColorAttachment(GLuint slot, GLuint channels, const glm::vec4 &clearColor)
  {
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");
    VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Invalid slot {}. Valid slots are 0-{}", slot, s_MaxColorAttachments - 1);
    VRM_ASSERT_MSG(!isColorAttachmentUsed(slot), "Slot {} is used", slot);

    if (m_samples > 1)
    {
      m_colorTextures.at(slot).createColorsMultisample(m_width, m_height, channels, m_samples);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D_MULTISAMPLE, m_colorTextures.at(slot).getRendererID(), 0);
    }
    else
    {
      m_colorTextures.at(slot).createColors(m_width, m_height, channels);
      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, m_colorTextures.at(slot).getRendererID(), 0);
    }


    m_clearColors.at(slot) = clearColor;
  }

  inline const Texture2D &FrameBuffer::getColorAttachmentTexture(GLuint slot) const
  {
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");
    VRM_ASSERT_MSG(slot < s_MaxColorAttachments, "Invalid slot {}. Valid slots are 0-{}", slot, s_MaxColorAttachments - 1);
    VRM_ASSERT_MSG(isColorAttachmentUsed(slot), "Slot {} is not used", slot);

    return m_colorTextures.at(slot);
  }

  inline constexpr bool FrameBuffer::isDepthAttachmentUsed() const
  {
    return m_depthTexture.isCreated() || m_isDefault;
  }

  inline void FrameBuffer::addDepthAttachment(float clearValue)
  {
    VRM_ASSERT_MSG(m_samples == 1, "You cannot add a depth attachment to a multisampled framebuffer. Use attachRenderBuffer() instead");
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");
    VRM_ASSERT_MSG(!m_depthTexture.isCreated(), "Framebuffer already has depth attachment");

    m_depthTexture.createDepth(m_width, m_height);

    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture.getRendererID(), 0);

    m_depthClearValue = clearValue;
  }

  inline const Texture2D &FrameBuffer::getDepthAttachmentTexture() const
  {
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");
    VRM_ASSERT_MSG(m_depthTexture.isCreated(), "Framebuffer has no depth attachment");

    return m_depthTexture;
  }

  inline constexpr bool FrameBuffer::isRenderBufferAttached() const
  {
    return m_renderBuffer.isCreated();
  }

  inline void FrameBuffer::attachRenderBuffer(float clearValue)
  {
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");
    VRM_ASSERT_MSG(!m_renderBuffer.isCreated(), "Framebuffer already has depth attachment");

    m_renderBuffer.create(m_width, m_height, m_samples);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer.getRenderID());

    m_depthClearValue = clearValue;
  }

  inline bool FrameBuffer::validate()
  {
    // Drawbuffer
    std::array<GLenum, s_MaxColorAttachments> drawBuffer;
    for (GLuint slot = 0; slot < s_MaxColorAttachments; ++slot)
    {
      if (isColorAttachmentUsed(slot))
      {
        drawBuffer.at(slot) = GL_COLOR_ATTACHMENT0 + slot;
      }
      else
      {
        drawBuffer.at(slot) = GL_NONE;
      }
    }

    glDrawBuffers(s_MaxColorAttachments, drawBuffer.data());

    GLenum errCode = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

    if (errCode == GL_FRAMEBUFFER_COMPLETE)
    {
      return true;
    }

    VRM_LOG_WARN("Framebuffer incomplete");
    return false;
  }

  inline bool FrameBuffer::resize(GLuint width, GLuint height)
  {
    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created (or is default)");

    if (m_width == width && m_height == height)
    {
      return true;
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
        colTex.release();
        addColorAttachment(slot, channels, m_clearColors.at(slot));
        // colTex.createColors(width, height, colTex.getChannels());
        // glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, m_colorTextures.at(slot).getRendererID(), 0);
      }
    }

    if (isDepthAttachmentUsed())
    {
      m_depthTexture.createDepth(width, height);
      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture.getRendererID(), 0);
    }

    if (isRenderBufferAttached())
    {
      m_renderBuffer.create(width, height, m_samples);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer.getRenderID());
    }

    return validate();
  }

  inline void FrameBuffer::clearColors() const
  {
    if (m_isDefault)
    {
      clearColors(glm::vec4{0.f, 0.f, 0.f, 1.f});
      return;
    }

    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created");

    for (GLuint slot = 0; slot < s_MaxColorAttachments; ++slot)
    {
      if (isColorAttachmentUsed(slot))
      {
        glClearBufferfv(GL_COLOR, slot, &m_clearColors.at(slot).r);
      }
    }
  }

  inline void FrameBuffer::clearColors(const glm::vec4 &customClearColor) const
  {
    if (m_isDefault)
    {
      glClearBufferfv(GL_COLOR, 0, &customClearColor.r);
      return;
    }

    VRM_ASSERT_MSG(isCreated(), "This framebuffer is not created");

    for (GLuint slot = 0; slot < s_MaxColorAttachments; ++slot)
    {
      if (isColorAttachmentUsed(slot))
      {
        glClearBufferfv(GL_COLOR, slot, &customClearColor.r);
      }
    }
  }

  inline void FrameBuffer::clearDepth() const
  {
    if (m_isDefault)
    {
      clearDepth(1.f);
    }
    else
    {
      clearDepth(m_depthClearValue);
    }
  }

  inline void FrameBuffer::clearDepth(float customClearDepth) const
  {
    VRM_ASSERT_MSG(isCreated() || m_isDefault, "This framebuffer is not created");

    if (isDepthAttachmentUsed() || isRenderBufferAttached())
    {
      glClearBufferfv(GL_DEPTH, 0, &customClearDepth);
    }

  }

} // namespace vrm::gl
