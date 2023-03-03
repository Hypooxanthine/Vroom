#pragma once

#include <SFML/Graphics/View.hpp>

#include <entt/entt.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Scene/Components.h"

namespace Vroom
{
	class Entity;
	class ChunkManager;

	class Scene
	{
		friend class Application;
		friend Entity;
		friend TransformComponent;

	public: // Public methods
		Scene(const sf::FloatRect& sceneArea = {-10000, -10000, 20000, 20000});
		virtual ~Scene();

		Entity createEntity();
		Entity createEntity(const std::string& name);

		Entity getEntity(entt::entity handle);
		Entity getEntityWithName(const std::string& name);

		template <typename C>
		entt::view<C> getAllEntitiesWithComponent()
		{
			return m_Registry.view<C>();
		}

		void setCamera(CameraComponent& camera);

	protected: // Protected methods

		inline AssetManager& getAssetManager() { return m_AssetManager; }
		inline const AssetManager& getAssetManager() const { return m_AssetManager; }

		// Virtual functions to override by client
		virtual void onSceneLoaded() {}
		virtual void onSceneUnloaded() {}
		virtual void onUpdate() {}
		virtual void onRender() {}

	private: // Private methids
		void setupRenderChunks();
		void renderChunksOutline(sf::RenderTarget& target);
		void renderCameraBox(sf::RenderTarget& target, const sf::FloatRect& cameraViewport);

	private: // Private methods called by the Application
		void render();
		void update();
		void init();
		void end();

		void notifyRenderComponent(SpriteComponent& sprite);

	private: // Private members
		sf::FloatRect m_SceneArea;

		// Entities
		entt::registry m_Registry;
		std::unique_ptr<ChunkManager> m_RenderChunks;

		// Assets
		AssetManager m_AssetManager;

		// Camera
		sf::View* m_Camera = nullptr;
	};
}