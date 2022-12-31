#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

namespace Vroom
{
	class SpriteAsset;

	class Sprite : public DynamicAsset
	{
	public:
		using StaticType = SpriteAsset;

	public:
		Sprite() = default;
		Sprite(const sf::Sprite& sprite);
		virtual ~Sprite();

		inline const sf::Sprite& getSprite() const { return m_Sprite; }
		inline sf::Sprite& getSprite() { return m_Sprite; }

	private:
		sf::Sprite m_Sprite;
	};
}