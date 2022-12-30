#include "Vroom/Asset/DynamicAssets/Texture.h"

#include "Vroom/Core/Log.h"

namespace Vroom
{
	Texture::Texture(const sf::Texture& texture)
		: DynamicAsset(), m_Texture(texture)
	{

	}
	Texture::~Texture()
	{
		LOG_TRACE("Texture instance deleted.");
	}
}