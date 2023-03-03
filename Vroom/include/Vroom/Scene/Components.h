#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <entt/entt.hpp>

#include "Vroom/Core/Minimal.h"
#include "Vroom/Asset/DynamicAssets/Sprite.h"

namespace Vroom
{
	class Scene;
	class Entity;
	
	class Component
	{
		friend Entity;
		friend Scene;
		friend class ScriptComponent;
	public:
		Component() = default;

		Scene& getScene() { return *m_Scene; }
		const Scene& getScene() const { return *m_Scene; }

		Entity getEntity() const;

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};

	class NameComponent : public Component
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

	class TransformComponent : public Component
	{
	public:
		TransformComponent()
			: m_Rotation(0.f), m_Scale(1.f, 1.f)
		{}

		const sf::Vector2f& getTranslation() const { return m_Translation; }
		float getRotation() const { return m_Rotation; }
		const sf::Vector2f& getScale() const { return m_Scale; }

		void setTranslation(const sf::Vector2f& pos) { m_Translation = pos; notifyMove(); }
		void setTranslation(float x, float y) { m_Translation = { x, y }; notifyMove(); }
		void setScale(const sf::Vector2f& scale) { m_Scale = scale; notifyMove(); }
		void setScale(float x, float y) { m_Scale = { x, y }; notifyMove(); }
		void setScale(float xy) { m_Scale = { xy, xy }; notifyMove(); }
		void setRotation(float rotation) { m_Rotation = rotation; notifyMove(); }

	private:
		void notifyMove();

	private:
		sf::Vector2f m_Translation = { 0.f, 0.f };
		float m_Rotation = 0.f;
		sf::Vector2f m_Scale = { 1.f, 1.f };

		bool m_HasMoved = false;
	};

	class SpriteComponent : public TransformComponent
	{
		friend Scene;
	public:
		SpriteComponent(std::unique_ptr<Sprite>&& sprite)
			: TransformComponent(), m_Sprite(std::move(sprite))
		{}

		void setSprite(std::unique_ptr<Sprite>&& sprite) { m_Sprite = std::move(sprite); }
		inline sf::FloatRect getRawBoundingBox() const { return m_Sprite->getSprite().getLocalBounds(); }

	private:
		std::unique_ptr<Sprite> m_Sprite = std::make_unique<Sprite>();
	};

	class CameraComponent : public TransformComponent
	{
		friend Scene;
	public:
		CameraComponent();

		void setSize(const sf::Vector2f& size);
		void setSize(float x, float y);

		void activate();

	private: // For internal use
		void updatePosition();

	private:
		sf::View m_View;
	};

	class ColliderComponent : public Component
	{
		friend Scene;

	public:
		ColliderComponent() = default;

		void setPosition(const sf::Vector2f& position);
		void setSize(const sf::Vector2f& size);

		sf::Vector2f getPosition() const;
		sf::Vector2f getSize() const;

	private:
		sf::FloatRect m_Rect;
	};

	class ScriptComponent : public Component
	{
		friend Scene;
		friend Entity;

	public:
		ScriptComponent() = default;

		virtual void onSpawn() {}
		virtual void onUpdate() {}

	protected:
		// Core
		float getDeltaTime() const;
		void quitGame() const;

		// Events
		bool getEventState(const std::string& eventName) const;
	};

	struct ScriptHandler
	{
		ScriptHandler(ScriptComponent* script)
			: script(std::unique_ptr<ScriptComponent>(script))
		{}

		std::unique_ptr<ScriptComponent> script;
	};
}