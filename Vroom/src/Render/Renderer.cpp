#include "Vroom/Render/Renderer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

/**
 * Frame buffer drawing setup code mostly from learnopengl.com 
 */

Renderer::Renderer()
{
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    GLCall(glGenTextures(1, &m_RenderTexture));
    GLCall(glGenFramebuffers(1, &m_Fbo));
    GLCall(glGenRenderbuffers(1, &m_Rbo));
}

Renderer::~Renderer()
{
    GLCall(glDeleteTextures(1, &m_RenderTexture));
    GLCall(glDeleteFramebuffers(1, &m_Fbo));
    GLCall(glDeleteRenderbuffers(1, &m_Rbo));
}

void Renderer::beginScene()
{
    GLCall(glViewport(m_ViewportOrigin.x, m_ViewportOrigin.y, m_ViewportSize.x, m_ViewportSize.y));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderTexture));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ViewportSize.x, m_ViewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture, 0));

    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_ViewportSize.x, m_ViewportSize.y));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_WARN("Frame Buffer not complete.");

    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::endScene()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::drawTriangles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Binding data
    va.bind();
    ib.bind();
    shader.bind();

    // Drawing data
    GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Binding data
    va.bind();
    ib.bind();
    shader.bind();

    // Drawing data
    GLCall(glDrawElements(GL_POINTS, (GLsizei)ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

const glm::vec<2, unsigned int>& Renderer::getViewportSize() const
{
    return m_ViewportSize;
}

void Renderer::setViewport(const glm::vec<2, unsigned int>& o, const glm::vec<2, unsigned int>& s)
{
    setViewportOrigin(o);
    setViewportSize(s);
}

void Renderer::setViewportOrigin(const glm::vec<2, unsigned int>& o)
{
    m_ViewportOrigin = o;
}

void Renderer::setViewportSize(const glm::vec<2, unsigned int>& s)
{
    m_ViewportSize = s;
}
