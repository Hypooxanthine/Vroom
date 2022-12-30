#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

#include "Vroom/Core/Minimal.h"
#include "Vroom/Asset/DynamicAssets/Sprite.h"

namespace Vroom
{
	struct NameComponent
	{
		std::string name;
	};

	struct TransformComponent
	{
		sf::Vector2f translation = { 0.f, 0.f };
		float rotation = 0.f;
		sf::Vector2f scale = { 1.f, 1.f };
	};

	struct SpriteComponent
	{
		std::unique_ptr<Sprite> sprite;
	};

	class Scene;
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