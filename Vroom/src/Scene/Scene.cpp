#include "Vroom/Scene/Scene.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components.h"
#include "Vroom/Scene/ChunkManager.h"

namespace Vroom
{
	Scene::Scene(const sf::FloatRect& sceneArea)
		: m_SceneArea(sceneArea),
		m_RenderChunks(nullptr),
		m_AssetManager(30ULL * 1024ULL * 1024ULL, 10ULL * 1024ULL * 1024ULL)
	{
		setCamera(createEntity().addComponent<CameraComponent>());
	}

	Scene::~Scene()
	{
	}

	Entity Scene::createEntity()
	{
		return createEntity("Unnamed Entity");
	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity out = Entity(m_Registry.create(), this);
		out.addComponent<TransformComponent>();
		out.addComponent<NameComponent>(name);

		return out;
	}

	Entity Scene::getEntity(entt::entity handle)
	{
		return Entity(handle, this);
	}

	Entity Scene::getEntityWithName(const std::string& name)
	{
		auto nameView = m_Registry.view<NameComponent>();

		for (auto entity : nameView)
		{
			auto& nameComponent = nameView.get<NameComponent>(entity);

			if (nameComponent.getName() == name)
				return Entity(entity, this);
		}

		return Entity();
	}

	void Scene::setCamera(CameraComponent& camera)
	{
		m_Camera = &camera.m_View;
		//Application::Get().getWindow().setView(*m_Camera);
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

	void Scene::setupRenderChunks()
	{
		LOG_INFO("Setting up render chunks with scene render components.");
		auto group = m_Registry.group<SpriteComponent, TransformComponent>();

		const float minChunkSize = 64.f;
		float leftMost = 0.f, rightMost = 0.f, upMost = 0.f, downMost = 0.f;
		float chunkSize = 0.f;
		auto it = group.begin();

		// Initialize variables with the first sprite
		if (it != group.end())
		{
			auto& sprite = m_Registry.get<SpriteComponent>(*it);
			auto& transform = m_Registry.get<TransformComponent>(*it);
			sf::Vector2f translation = transform.getTranslation() + sprite.getTranslation();
			sf::Vector2f scale = { transform.getScale().x * sprite.getScale().x, transform.getScale().y * sprite.getScale().y };
			sf::FloatRect rawBox = sprite.getRawBoundingBox();
			sf::FloatRect box = { rawBox.left + translation.x, rawBox.top + translation.y, rawBox.width * scale.x, rawBox.height * scale.y };

			leftMost = box.left;
			rightMost = box.left + box.width;
			upMost = box.top;
			downMost = box.top + box.height;

			it++;

			for (it; it != group.end(); it++)
			{
				auto& sprite = m_Registry.get<SpriteComponent>(*it);
				auto& transform = m_Registry.get<TransformComponent>(*it);
				sf::Vector2f translation = transform.getTranslation() + sprite.getTranslation();
				sf::Vector2f scale = { transform.getScale().x * sprite.getScale().x, transform.getScale().y * sprite.getScale().y };
				sf::FloatRect rawBox = sprite.getRawBoundingBox();
				sf::FloatRect box = { rawBox.left + translation.x, rawBox.top + translation.y, rawBox.width * scale.x, rawBox.height * scale.y };

				if (box.left < leftMost) leftMost = box.left;
				if (box.left + box.width > rightMost) rightMost = box.left + box.width;
				if (box.top < upMost) upMost = box.top;
				if (box.top + box.height > downMost) downMost = box.top + box.height;
			}

			float width = rightMost - leftMost, height = downMost - upMost;
			chunkSize = std::max(std::max(width, height) / 100.f, minChunkSize);
		}
		else
		{
			LOG_TRACE("No entity in scene.");

			chunkSize = std::max(std::max(m_SceneArea.width, m_SceneArea.height) / 100.f, minChunkSize);
		}

		m_RenderChunks = std::make_unique<ChunkManager>(
			chunkSize, chunkSize,
			int(m_SceneArea.left / chunkSize), int((m_SceneArea.left + m_SceneArea.width) / chunkSize),
			int(m_SceneArea.top / chunkSize), int((m_SceneArea.top + m_SceneArea.height) / chunkSize));

		LOG_INFO("Render chunks initialized width size:{}. Chunks number x:{}, y:{}.",
			chunkSize, int(m_SceneArea.width / chunkSize), int(m_SceneArea.height / chunkSize));

		LOG_INFO("Adding {} entities to the chunk manager.", group.size());

		for (auto entity : group)
		{
			m_RenderChunks->addEntityBySprite(Entity(entity, this));
		}

		LOG_INFO("Entities added.");
	}

	void Scene::render()
	{
		sf::RenderTexture tex;
		auto& window = Application::Get().getWindow();
		tex.create(window.getSize().x, window.getSize().y, window.getSettings());
		tex.clear();
		tex.setView(*m_Camera);

		// Sprites
		sf::FloatRect cameraViewport = {
			m_Camera->getCenter().x - m_Camera->getSize().x / 2.f,
			m_Camera->getCenter().y - m_Camera->getSize().y / 2.f,
			m_Camera->getSize().x,
			m_Camera->getSize().y
		};

		auto entities = m_RenderChunks->getEntitiesInField(
			cameraViewport.left, cameraViewport.left + cameraViewport.width,
			cameraViewport.top, cameraViewport.top + cameraViewport.height
		);

		for (auto& entity : entities)
		{
			auto& sc = entity.getComponent<SpriteComponent>();
			auto& tc = entity.getComponent<TransformComponent>();

			sc.m_Sprite->getSprite().setPosition(sc.getTranslation() + tc.getTranslation());
			sc.m_Sprite->getSprite().setScale(sc.getScale().x * tc.getScale().x, sc.getScale().y * tc.getScale().y);
			sc.m_Sprite->getSprite().setRotation(sc.getRotation() + tc.getRotation());

			tex.draw(sc.m_Sprite->getSprite());
		}

		//renderChunksOutline(tex);

		//renderCameraBox(tex, cameraViewport);

		tex.display();
		sf::Sprite displaySprite;
		displaySprite.setTexture(tex.getTexture());
		window.draw(displaySprite);

		onRender();
	}

	void Scene::renderChunksOutline(sf::RenderTarget& target)
	{
		sf::RectangleShape shapeHorizontal(sf::Vector2f(m_SceneArea.width, 1.f));
		shapeHorizontal.setFillColor(sf::Color::Red);
		shapeHorizontal.setOutlineColor(sf::Color::Red);
		sf::RectangleShape shapeVertical(sf::Vector2f(1.f, m_SceneArea.height));
		shapeVertical.setFillColor(sf::Color::Red);
		shapeVertical.setOutlineColor(sf::Color::Red);

		auto chunksNumber = m_RenderChunks->getChunksNumber();

		for (int i = 0; i <= chunksNumber.x; i++)
		{
			shapeVertical.setPosition(i * m_RenderChunks->getChunkSize().x + m_SceneArea.left, m_SceneArea.top);
			target.draw(shapeVertical);
		}

		for (int i = 0; i <= chunksNumber.y; i++)
		{
			shapeHorizontal.setPosition(m_SceneArea.left, i * m_RenderChunks->getChunkSize().y + m_SceneArea.top);
			target.draw(shapeHorizontal);
		}
	}

	void Scene::renderCameraBox(sf::RenderTarget& target, const sf::FloatRect& cameraViewport)
	{
		sf::RectangleShape cameraShape({ cameraViewport.width, cameraViewport.height });
		cameraShape.setPosition(cameraViewport.left, cameraViewport.top);
		cameraShape.setOutlineColor(sf::Color::Blue);
		cameraShape.setOutlineThickness(1.f);
		cameraShape.setFillColor(sf::Color::Transparent);
		target.draw(cameraShape);
	}

	void Scene::init()
	{
		LOG_INFO("Initializing scene.");

		setupRenderChunks();

		onSceneLoaded();
	}

	void Scene::end()
	{
		onSceneUnloaded();
	}
	void Scene::notifyRenderComponent(SpriteComponent& sprite)
	{
	}
}