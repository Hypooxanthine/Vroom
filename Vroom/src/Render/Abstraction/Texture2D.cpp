#include "Vroom/Render/Abstraction/Texture2D.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Core/Assert.h"

namespace vrm
{

static constexpr GLenum ToGlFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::Grayscale:
        return GL_RED;
    case TextureFormat::RG:
        return GL_RG;
    case TextureFormat::RGB:
        return GL_RGB;
    case TextureFormat::RGBA:
        return GL_RGBA;
    default:
        VRM_ASSERT_MSG(false, "Unsupported format.");
        return 0;
    }
}

static constexpr GLint ToGlInternalFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::Grayscale:
        return GL_R8;
    case TextureFormat::RG:
        return GL_RG8;
    case TextureFormat::RGB:
        return GL_RGB8;
    case TextureFormat::RGBA:
        return GL_RGBA8;
    default:
        VRM_ASSERT_MSG(false, "Unsupported format.");
        return 0;
    }
}

Texture2D::Texture2D()
{}

Texture2D::~Texture2D()
{
    release();
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other)
    {
        m_RendererID = other.m_RendererID;

        other.m_RendererID = 0;
    }

    return *this;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    *this = std::move(other);
}

void Texture2D::bind(unsigned int slot) const
{
    VRM_DEBUG_ASSERT_MSG(isCreated(), "Texture not created.");
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::createColors(int width, int height, int channels, const void* data)
{
    if (!isCreated())
        GLCall(glGenTextures(1, &m_RendererID));

    bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        ToGlInternalFormat(ChannelsToTextureFormat(channels)),
        width,
        height,
        0,
        ToGlFormat(ChannelsToTextureFormat(channels)),
        GL_UNSIGNED_BYTE,
        data
    ));
}

void Texture2D::createFloats(int width, int height, int channels, const void* data)
{
    if (!isCreated())
        GLCall(glGenTextures(1, &m_RendererID));

    bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        ToGlInternalFormat(ChannelsToTextureFormat(channels)),
        width,
        height,
        0,
        ToGlFormat(ChannelsToTextureFormat(channels)),
        GL_FLOAT,
        data
    ));
}

void Texture2D::createDepth(int width, int height)
{
    if (!isCreated())
        GLCall(glGenTextures(1, &m_RendererID));

    bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        width,
        height,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
    ));
}

void Texture2D::release()
{
    if (isCreated())
    {
        GLCall_nothrow(glDeleteTextures(1, &m_RendererID));
        m_RendererID = 0;
    }
}

void Texture2D::loadFromTextureData(const ByteTextureData& textureData)
{
    createColors(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), textureData.getData());
}

void Texture2D::loadFromTextureData(const FloatTextureData& textureData)
{
    createFloats(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), textureData.getData());
}

} // namespace vrm