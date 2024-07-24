#include "Vroom/Render/Abstraction/Texture2D.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Core/Assert.h"

static constexpr GLenum toGLFormat(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::RGB: return GL_RGB;
    case Texture2D::Format::RGBA: return GL_RGBA;
    default: return GL_RGB;
    }
}

static constexpr GLenum toGLInternalFormat(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::RGB: return GL_RGB8;
    case Texture2D::Format::RGBA: return GL_RGBA8;
    default: return GL_RGB8;
    }
}

Texture2D::Texture2D()
{
    GLCall(glGenTextures(1, &m_RendererID));
}

Texture2D::~Texture2D()
{
    GLCall_nothrow(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::create(int width, int height, Format format)
{
    m_Width = width;
    m_Height = height;

    bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, toGLInternalFormat(format), m_Width, m_Height, 0, toGLFormat(format), GL_UNSIGNED_BYTE, nullptr));
    unbind();
}
