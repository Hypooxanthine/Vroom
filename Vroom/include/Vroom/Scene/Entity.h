#pragma once

#include <entt/entt.hpp>

#include "Vroom/Core/Assert.h"
#include "Vroom/Scene/Scene.h"
#include "Vroom/Scene/Components.h"

namespace Vroom
{
	class Entity
	{
	public:
		Entity() = delete;
		Entity(entt::entity id, Scene* scene);

		template <typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			VR_ASSERT_MSG(!containsComponent<T>(), "Component already exists.");

			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& addScriptComponent()
		{
			VR_ASSERT_MSG(!containsComponent<ScriptHandler>(), "A ScriptComponent already exists.");
			static_assert(std::is_base_of<ScriptComponent, T>::value);
			
			T* component = new T();
			m_Scene->m_Registry.emplace<ScriptHandler>(m_Handle, component);
			ScriptComponent* scriptComponent = m_Scene->m_Registry.get<ScriptHandler>(m_Handle).script.get();
			scriptComponent->m_Handle = m_Handle;
			scriptComponent->m_Scene = m_Scene;

			return *component;
		}

		template <typename T>
		T& getComponent()
		{
			VR_ASSERT_MSG(containsComponent<T>(), "Component does not exist.");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template <typename T>
		bool containsComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Handle);
		}

		inline entt::entity getHandle() const { return m_Handle; }

		inline Scene* getScene() { return m_Scene; }
		inline const Scene* getScene() const { return m_Scene; }

	private: // Private members
		entt::entity m_Handle;
		Scene* m_Scene = nullptr;
	};
}