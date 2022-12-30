#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Vroom/Core/Minimal.h"

#include "Vroom/Asset/StaticAssets/StaticAsset.h"
#include "Vroom/Asset/DynamicAssets/Sprite.h"

#include "Vroom/Asset/DynamicAssets/Texture.h"

namespace Vroom
{
	class SpriteAsset : public StaticAsset
	{
		friend AssetManager;

	public:
		virtual ~SpriteAsset();

	protected:
		SpriteAsset(AssetManager* manager, const std::string& path);

		virtual Sprite* onInstanciate() const override;

		virtual bool onLoad() override;

	private:
		std::unique_ptr<Texture> m_TextureDynAsset;
		sf::Sprite m_Sprite;
	};
}