#include "Vroom/Render/Renderer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

#include <glm/gtc/matrix_transform.hpp>

/**
 * Frame buffer drawing setup code mostly from learnopengl.com 
 */

Renderer::Renderer()
{
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Renderer::~Renderer()
{
}

void Renderer::beginScene()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::endScene()
{

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
