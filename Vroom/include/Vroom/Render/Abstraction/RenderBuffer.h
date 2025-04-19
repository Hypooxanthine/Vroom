#pragma once

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  class RenderBuffer
  {
  public:
    inline constexpr RenderBuffer();
    inline ~RenderBuffer();

    inline void bind() const;

    inline void create(GLuint width, GLuint height, GLuint samples = 1);

    inline void release();

    inline constexpr GLuint getRenderID() const;

    inline constexpr bool isCreated() const;

  private:
    GLuint m_renderID = 0;
    GLuint m_Width = 0, m_Height = 0;
    GLuint m_samples = 0;
  };

  inline constexpr RenderBuffer::RenderBuffer()
  {

  }

  inline RenderBuffer::~RenderBuffer()
  {
    if (isCreated())
    {
      glDeleteRenderbuffers(1, &m_renderID);
    }
  }

  inline void RenderBuffer::bind() const
  {
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderID);
  }

  inline void RenderBuffer::create(GLuint width, GLuint height, GLuint samples)
  {
    VRM_ASSERT_MSG(width != 0 && height != 0, "Width and height must be > 0");
    VRM_ASSERT_MSG(samples > 0, "Samples must be > 0");

    if (!isCreated())
    {
      glGenRenderbuffers(1, &m_renderID);
    }

    m_Width = width;
    m_Height = height;
    m_samples = samples;

    bind();
    if (m_samples > 1)
    {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    }
    else
    {
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    }
  }

  inline void RenderBuffer::release()
  {
    if (!isCreated())
    {
      return;
    }
    
    glDeleteRenderbuffers(1, &m_renderID);
    m_renderID = 0;
    m_Width = 0; m_Height = 0;
    m_samples = 0;
  }

  inline constexpr GLuint RenderBuffer::getRenderID() const
  {
    return m_renderID;
  }

  inline constexpr bool RenderBuffer::isCreated() const
  {
    return m_renderID != 0;
  }

} // namespace vrm::gl