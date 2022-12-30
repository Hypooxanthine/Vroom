#include "Vroom/Asset/StaticAssets/TextureAsset.h"

namespace Vroom
{
	TextureAsset::~TextureAsset()
	{
		LOG_INFO("TextureAsset unloaded : {}.", getPath());
	}

	Texture* TextureAsset::onInstanciate() const
	{
		LOG_TRACE("Instanciating a Texture : {}.", getPath());

		return new Texture(m_Texture);
	}

	bool TextureAsset::onLoad()
	{
		LOG_TRACE("Loading a TextureAsset : {}.", getPath());

		if (!m_Texture.loadFromFile(getPath())) return false;

		LOG_INFO("TextureAsset loaded : {}.", getPath());

		return true;
	}
}