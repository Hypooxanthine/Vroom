#include "Vroom/Render/Abstraction/FrameBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{

FrameBuffer::FrameBuffer()
{
    GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::FrameBuffer(const Specification& spec)
    : m_Specification(spec)
{
    create(spec);
}

FrameBuffer::~FrameBuffer()
{
    GLCall_nothrow(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    GLCall(glClearColor(m_Specification.clearColor.r, m_Specification.clearColor.g, m_Specification.clearColor.b, m_Specification.clearColor.a));

    if (m_Specification.useDepthTest)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    if (m_Specification.useBlending)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }
}

void FrameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::create(const Specification& spec)
{
    m_Specification = spec;
    if (m_Specification.onScreen)
    {
        m_RendererID = 0; // Using default framebuffer
        return;
    }
    

    m_Texture.create(m_Specification.width, m_Specification.height, Texture2D::Format::RGBA);
    m_RenderBuffer.create(m_Specification.width, m_Specification.height);

    bind();
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.getRendererID(), 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer.getRendererID()));

    VRM_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    unbind();
}

void FrameBuffer::clearColorBuffer() const
{
    GLbitfield mask = 0;
    if (m_Specification.useDepthTest)
        mask |= GL_DEPTH_BUFFER_BIT;
    if (m_Specification.useBlending)
        mask |= GL_COLOR_BUFFER_BIT;
        
    GLCall(glClear(mask));
}

} // namespace vrm