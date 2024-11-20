#include "Vroom/Render/Abstraction/RenderBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm
{

RenderBuffer::RenderBuffer()
{
    GLCall(glGenRenderbuffers(1, &m_RendererID));
}

RenderBuffer::~RenderBuffer()
{
    GLCall_nothrow(glDeleteRenderbuffers(1, &m_RendererID));
}

void RenderBuffer::bind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::unbind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::create(int width, int height)
{
    m_Width = width;
    m_Height = height;

    bind();
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
    unbind();
}

} // namespace vrm