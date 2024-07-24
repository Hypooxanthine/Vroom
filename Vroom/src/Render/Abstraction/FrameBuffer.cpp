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

void FrameBuffer::bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}