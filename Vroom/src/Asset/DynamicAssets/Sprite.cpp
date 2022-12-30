#include "Vroom/Asset/DynamicAssets/Sprite.h"

#include "Vroom/Core/Log.h"

namespace Vroom
{
	Sprite::Sprite(const sf::Sprite& sprite)
		: m_Sprite(sprite)
	{
	}
	Sprite::~Sprite()
	{
		LOG_TRACE("Sprite instance deleted.");
	}
}