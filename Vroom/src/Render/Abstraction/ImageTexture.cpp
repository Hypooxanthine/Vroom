#include "Vroom/Render/Abstraction/ImageTexture.h"

#include "stb_image/stb_image.h"
#include <string>

#include "Vroom/Render/Abstraction/GLCall.h"

ImageTexture::ImageTexture(const std::string& path)
	: ImageTexture()
{
	loadFromFile(path);
}

ImageTexture::ImageTexture()
	: Texture2D(), m_BPP(0), m_Loaded(false)
{

}

ImageTexture::~ImageTexture()
{

}

bool ImageTexture::loadFromFile(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	int width, height;
	unsigned char* localBuffer = stbi_load(path.c_str(), &width, &height, &m_BPP, 4);

	if (localBuffer == nullptr) return false;

	//std::cout << "Image loaded. Width:" << m_Width << ", Height:" << m_Height << ", BPP:" << m_BPP << std::endl;

	create(width, height, Format::RGBA);

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

	stbi_image_free(localBuffer);

	m_Loaded = true;
	return true;
}
