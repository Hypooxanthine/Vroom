#pragma once

#include <SFML/Graphics/View.hpp>

#include <entt/entt.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Scene/Components.h"

#ifndef _DEBUG
#define setShowChunks(x)  
#define setShowCamera(x)  
#endif

namespace Vroom
{
	class Entity;
	class ChunkManager;

	class Scene
	{
		friend class Application;
		friend Entity;
		friend TransformComponent;
		friend LayerComponent;

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

	// Debug
#ifdef _DEBUG
	public:
		inline bool getShowChunks() const { return m_ShowChunks; }
		inline bool getShowCamera() const { return m_ShowCamera; }
		inline void setShowChunks(bool val) { m_ShowChunks = val; }
		inline void setShowCamera(bool val) { m_ShowCamera = val; }
#endif

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
		void renderBox(sf::RenderTarget& target, const sf::FloatRect& rect, const sf::Color& color);

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

		// Debug
#ifdef _DEBUG
		bool m_ShowChunks = false;
		bool m_ShowCamera = false;
#endif
	};
}