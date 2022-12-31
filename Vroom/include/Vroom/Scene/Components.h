#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

#include "Vroom/Core/Minimal.h"
#include "Vroom/Asset/DynamicAssets/Sprite.h"

namespace Vroom
{
	class NameComponent
	{
	public:
		NameComponent(const std::string& name)
			: m_Name(name)
		{}

		NameComponent(std::string&& name)
			: m_Name(std::move(name))
		{}

		const std::string& getName() const { return m_Name; }

		void setName(const std::string& name) { m_Name = name; }

	private:
		std::string m_Name;
	};

	class TransformComponent
	{
	public:
		TransformComponent()
			: m_Rotation(0.f), m_Scale(1.f, 1.f)
		{}

		const sf::Vector2f& getTranslation() const { return m_Translation; }
		float getRotation() const { return m_Rotation; }
		const sf::Vector2f& getScale() const { return m_Scale; }

		void setTranslation(const sf::Vector2f& pos) { m_Translation = pos; }
		void setTranslation(float x, float y) { m_Translation = { x, y }; }
		void setScale(const sf::Vector2f& scale) { m_Scale = scale; }
		void setScale(float x, float y) { m_Scale = { x, y }; }
		void setScale(float xy) { m_Scale = { xy, xy }; }
		void setRotation(float rotation) { m_Rotation = rotation; }

	private:
		sf::Vector2f m_Translation = { 0.f, 0.f };
		float m_Rotation = 0.f;
		sf::Vector2f m_Scale = { 1.f, 1.f };
	};

	class Scene;

	class SpriteComponent : public TransformComponent
	{
		friend Scene;
	public:
		SpriteComponent(std::unique_ptr<Sprite>&& sprite)
			: TransformComponent(), m_Sprite(std::move(sprite))
		{}

		void setSprite(std::unique_ptr<Sprite>&& sprite) { m_Sprite = std::move(sprite); }

	private:
		std::unique_ptr<Sprite> m_Sprite = std::make_unique<Sprite>();
	};

	class Entity;

	class ScriptComponent
	{
		friend Scene;
		friend Entity;

	public:
		ScriptComponent() = default;

		virtual void onSpawn() {}
		virtual void onUpdate() {}

	protected:
		Scene* getScene();
		const Scene* getScene() const;

		Entity getEntity() const;

		float getDeltaTime() const;

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};

	struct ScriptHandler
	{
		ScriptHandler(ScriptComponent* script)
			: script(std::unique_ptr<ScriptComponent>(script))
		{}

		std::unique_ptr<ScriptComponent> script;
	};
}