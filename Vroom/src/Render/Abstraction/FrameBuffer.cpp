#include "Vroom/Render/Abstraction/FrameBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

FrameBuffer::FrameBuffer()
{
    GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
    GLCall_nothrow(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::create(int width, int height)
{
    m_Width = width;
    m_Height = height;

    m_Texture.create(width, height, Texture2D::Format::RGB);
    m_RenderBuffer.create(width, height);

    bind();
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.getRendererID(), 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer.getRendererID()));

    VRM_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    unbind();
}