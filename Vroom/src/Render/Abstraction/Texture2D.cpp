#include "Vroom/Render/Abstraction/Texture2D.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Core/Assert.h"

namespace vrm
{

static constexpr GLenum toGLFormat(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::Grayscale: return GL_RED;
    case Texture2D::Format::RGB: return GL_RGB;
    case Texture2D::Format::RGBA: return GL_RGBA;
    default: return GL_RGB;
    }
}

static constexpr GLenum toGLInternalFormat(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::Grayscale: return GL_R8;
    case Texture2D::Format::RGB: return GL_RGB8;
    case Texture2D::Format::RGBA: return GL_RGBA8;
    default: return GL_RGB8;
    }
}

static constexpr GLenum toGLType(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::Grayscale: return GL_UNSIGNED_BYTE;
    case Texture2D::Format::RGB: return GL_UNSIGNED_BYTE;
    case Texture2D::Format::RGBA: return GL_UNSIGNED_BYTE;
    default: return GL_UNSIGNED_BYTE;
    }
}

static constexpr int pixelChannelsCount(Texture2D::Format format)
{
    switch (format)
    {
    case Texture2D::Format::Grayscale: return 1;
    case Texture2D::Format::RGB: return 3;
    case Texture2D::Format::RGBA: return 4;
    default: return 4;
    }
}

static constexpr Texture2D::Format toTextureFormat(int channels)
{
    switch (channels)
    {
    case 1: return Texture2D::Format::Grayscale;
    case 3: return Texture2D::Format::RGB;
    case 4: return Texture2D::Format::RGBA;
    default: return Texture2D::Format::RGBA;
    }
}

static constexpr int bytesPerPixel(Texture2D::Format format)
{
    return pixelChannelsCount(format) * sizeof(unsigned char);
}

Texture2D::Texture2D()
{}

Texture2D::Texture2D(const std::string& path)
{
    loadFromFile(path);
}

Texture2D::~Texture2D()
{
    release();
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    if (this != &other)
    {
        if (other.isCreated())
        {
            // Create a new texture on GPU with empty data (no expensive getData() call).
            create(other.m_Width, other.m_Height, other.m_Format);
            // Copy data from other texture to this texture.
            GLCall(glCopyImageSubData(
                other.m_RendererID, GL_TEXTURE_2D, 0, 0, 0, 0,
                m_RendererID,       GL_TEXTURE_2D, 0, 0, 0, 0,
                m_Width, m_Height, 1));
        }
        else
            release();
    }

    return *this;
}

Texture2D::Texture2D(const Texture2D& other)
{
    *this = other;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other)
    {
        m_RendererID = other.m_RendererID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Format = other.m_Format;
        m_BPP = other.m_BPP;

        other.m_RendererID = 0;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_BPP = 0;
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

void Texture2D::create(int width, int height, Format format, const void* data)
{
    if (!isCreated())
        GLCall(glGenTextures(1, &m_RendererID));

    m_Width = width;
    m_Height = height;
    m_Format = format;
    m_BPP = bytesPerPixel(format);

    bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, toGLInternalFormat(format), m_Width, m_Height, 0, toGLFormat(format), toGLType(format), data));
}

void Texture2D::release()
{
    if (isCreated())
    {
        GLCall_nothrow(glDeleteTextures(1, &m_RendererID));
        m_RendererID = 0;
        m_Width = 0;
        m_Height = 0;
        m_BPP = 0;
    }
}

bool Texture2D::loadFromFile(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	int width, height;
    int channels;
	unsigned char* localBuffer = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (localBuffer == nullptr) return false;

	//std::cout << "Image loaded. Width:" << m_Width << ", Height:" << m_Height << ", BPP:" << m_BPP << std::endl;

	create(width, height, toTextureFormat(channels), localBuffer);
	stbi_image_free(localBuffer);

	return true;
}

bool Texture2D::saveToFile(const std::string& path)
{
    if (!isCreated())
    {
        VRM_LOG_WARN("Texture not created.");
        return false;
    }

    stbi_flip_vertically_on_write(1);

    int result = stbi_write_png(path.c_str(), m_Width, m_Height, m_BPP, getData().data(), m_Width * m_BPP);
    return result != 0;
}

std::vector<unsigned char> Texture2D::getData() const
{
    bind();
    std::vector<unsigned char> data(m_Width * m_Height * pixelChannelsCount(m_Format));
    GLCall(glGetTexImage(GL_TEXTURE_2D, 0, toGLFormat(m_Format), GL_UNSIGNED_BYTE, data.data()));
    return data;
}

} // namespace vrm