#include "Vroom/Scene/Scene.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components.h"

namespace Vroom
{
	Scene::Scene()
		: m_AssetManager(30ULL * 1024ULL * 1024ULL, 10ULL * 1024ULL * 1024ULL)
	{
		setCamera(createEntity().addComponent<CameraComponent>());
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

	void Scene::setCamera(CameraComponent& camera)
	{
		m_Camera = &camera.m_View;
		//Application::Get().getWindow().setView(*m_Camera);
	}

	void Scene::render()
	{
		sf::RenderTexture tex;
		auto& window = Application::Get().getWindow();
		tex.create(window.getSize().x, window.getSize().y, window.getSettings());
		tex.clear();
		tex.setView(*m_Camera);

		// Sprites
		auto group = m_Registry.group<SpriteComponent, TransformComponent>();
		group.each
		(
			[&tex](SpriteComponent& sc, TransformComponent& tc)
			{
				sc.m_Sprite->getSprite().setPosition(sc.getTranslation() + tc.getTranslation());
				sc.m_Sprite->getSprite().setScale(sc.getScale().x * tc.getScale().x, sc.getScale().y * tc.getScale().y);
				sc.m_Sprite->getSprite().setRotation(sc.getRotation() + tc.getRotation());

				tex.draw(sc.m_Sprite->getSprite());

			}
		);
		tex.display();
		sf::Sprite displaySprite;
		displaySprite.setTexture(tex.getTexture());
		window.draw(displaySprite);

		onRender();
	}

	void Scene::update()
	{
		// Scripts
		{
			auto view = m_Registry.view<ScriptHandler>();

			for (auto e : view)
			{
				m_Registry.get<ScriptHandler>(e).script->onUpdate();
			}
		}

		// Cameras
		{
			auto view = m_Registry.view<CameraComponent>();

			for (auto e : view)
			{
				m_Registry.get<CameraComponent>(e).updatePosition();
			}

			//Application::Get().getWindow().setView(*m_Camera);
		}

		onUpdate();
	}
}