#include "Vroom/Asset/StaticAssets/SpriteAsset.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAssets/TextureAsset.h"

#include <fstream>

namespace Vroom
{
	SpriteAsset::~SpriteAsset()
	{
		LOG_INFO("SpriteAsset unloaded : {}.", getPath());
	}

	SpriteAsset::SpriteAsset(AssetManager* manager, const std::string& path)
		: StaticAsset(manager, path), m_TextureDynAsset(nullptr)
	{

	}

	Sprite* SpriteAsset::onInstanciate() const
	{
		LOG_TRACE("Instanciating a Sprite : {}.", getPath());

		return new Sprite(m_Sprite);
	}

	bool SpriteAsset::onLoad()
	{
		LOG_TRACE("Loading a SpriteAsset : {}.", getPath());

		std::ifstream ifs;

		ifs.open(getPath());

		if (!ifs.is_open()) return false;

		std::string line;

		// First line : texture path
		if (!std::getline(ifs, line)) return false;
		m_TextureDynAsset = getAssetManager().getAsset<Texture>(line);
		m_Sprite.setTexture(m_TextureDynAsset->getTexture());

		// Second line : texture rect
		if (!std::getline(ifs, line)) return false;
		std::istringstream ss(line);
		sf::IntRect texture_rect;
		ss >> texture_rect.left >> texture_rect.top >> texture_rect.width >> texture_rect.height;
		m_Sprite.setTextureRect(texture_rect);

		LOG_INFO("SpriteAsset loaded : {}.", getPath());

		return true;
	}
}