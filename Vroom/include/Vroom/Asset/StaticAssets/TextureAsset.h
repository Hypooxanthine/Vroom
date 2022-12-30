#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Vroom/Core/Minimal.h"

#include "Vroom/Asset/StaticAssets/StaticAsset.h"
#include "Vroom/Asset/DynamicAssets/Texture.h"

namespace Vroom
{
	class TextureAsset : public StaticAsset
	{
		friend AssetManager;

	public:
		virtual ~TextureAsset();

	protected:
		TextureAsset(AssetManager* manager, const std::string& path) : StaticAsset(manager, path) {}

		virtual Texture* onInstanciate() const override;

		virtual bool onLoad() override;

	private:
		sf::Texture m_Texture;
	};
}