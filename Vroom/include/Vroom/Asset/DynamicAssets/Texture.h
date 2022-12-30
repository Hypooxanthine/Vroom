#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

namespace Vroom
{
	class TextureAsset;

	class Texture : public DynamicAsset
	{
	public:
		using StaticType = TextureAsset;

	public:
		Texture(const sf::Texture& texture);
		virtual ~Texture();

		inline const sf::Texture& getTexture() const { return m_Texture; }

	private:
		const sf::Texture& m_Texture;
	};
}