#pragma once

#include <entt/entt.hpp>

#include "Vroom/Asset/AssetManager.h"

#include "Vroom/Scene/Components.h"

namespace Vroom
{
	class Entity;

	class Scene
	{
		friend class Application;
		friend Entity;

	public: // Public methods
		Scene();
		virtual ~Scene();

		Entity createEntity();

		Entity getEntity(entt::entity handle);

	protected: // Protected methods

		inline AssetManager& getAssetManager() { return m_AssetManager; }
		inline const AssetManager& getAssetManager() const { return m_AssetManager; }

		// Virtual functions to override by client
		virtual void onSceneLoaded() {}
		virtual void onSceneUnloaded() {}
		virtual void onRender() {}
		virtual void onUpdate() {}

	private: // Private methods called by the Application
		void render();
		void update();

	private: // Private members
		// Entities
		entt::registry m_Registry;

		// Assets
		AssetManager m_AssetManager;
	};
}