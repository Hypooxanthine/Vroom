#include "Vroom/Scene/Scene.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components.h"

namespace Vroom
{
	Scene::Scene()
		: m_AssetManager(30ULL * 1024ULL * 1024ULL, 10ULL * 1024ULL * 1024ULL)
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::createEntity()
	{
		Entity out = Entity(m_Registry.create(), this);
		m_Registry.emplace<TransformComponent>(out.getHandle());

		return out;
	}

	Entity Scene::getEntity(entt::entity handle)
	{
		return Entity(handle, this);
	}

	void Scene::render()
	{
		auto group = m_Registry.group<SpriteComponent, TransformComponent>();
		group.each
		(
			[](SpriteComponent& sc, TransformComponent& tc)
			{
				sc.sprite->getSprite().setPosition(tc.translation);
				sc.sprite->getSprite().setScale(tc.scale);
				sc.sprite->getSprite().setRotation(tc.rotation);

				Application::Get().getWindow().draw(sc.sprite->getSprite());
			}
		);

		onRender();
	}

	void Scene::update()
	{
		auto view = m_Registry.view<ScriptHandler>();

		for (auto e : view)
		{
			m_Registry.get<ScriptHandler>(e).script->onUpdate();
		}

		onUpdate();
	}
}