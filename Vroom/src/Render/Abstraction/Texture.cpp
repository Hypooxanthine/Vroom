#include "Vroom/Render/Abstraction/Texture.h"

#include "stb_image/stb_image.h"
#include <string>

#include "Vroom/Render/Abstraction/GLCall.h"

Texture::Texture(const std::string& path)
	: Texture()
{
	loadFromFile(path);
}

Texture::Texture()
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Loaded(false)
{

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

bool Texture::loadFromFile(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (m_LocalBuffer == nullptr) return false;

	//std::cout << "Image loaded. Width:" << m_Width << ", Height:" << m_Height << ", BPP:" << m_BPP << std::endl;

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	stbi_image_free(m_LocalBuffer);

	m_Loaded = true;
	return true;
}

void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
