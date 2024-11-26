#include "Vroom/Render/Abstraction/FrameBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{

FrameBuffer::FrameBuffer()
{
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
    
    GLCall(glGenFramebuffers(1, &m_RendererID));

    m_Texture.createColors(m_Specification.width, m_Specification.height, 4);
    m_RenderBuffer.create(m_Specification.width, m_Specification.height);

    bind();
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.getRendererID(), 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer.getRendererID()));

    VRM_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    unbind();
}

void FrameBuffer::setOnScreenRender(bool onScreen)
{
    if (onScreen == m_Specification.onScreen)
        return;
    
    // If we are switching from off screen to on screen, we need to reset the framebuffer
    // because we had allocated a framebuffer for off screen rendering.
    if (!m_Specification.onScreen)
        reset();
        
    auto newSpec = m_Specification;
    newSpec.onScreen = onScreen;

    create(newSpec);
}

void FrameBuffer::resize(int width, int height)
{
    if (m_Specification.width == width && m_Specification.height == height)
        return;
        
    m_Specification.width = width;
    m_Specification.height = height;

    if (m_Specification.onScreen)
        return;

    m_Texture.createColors(m_Specification.width, m_Specification.height, 4);
    m_RenderBuffer.create(m_Specification.width, m_Specification.height);
}

void FrameBuffer::setClearColor(const glm::vec4& color)
{
    m_Specification.clearColor = color;
}

void FrameBuffer::reset()
{
    /// @todo This should also reset the texture and renderbuffer.
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
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