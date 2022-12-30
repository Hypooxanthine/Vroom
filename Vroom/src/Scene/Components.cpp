#include "Vroom/Scene/Components.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Scene.h"
#include "Vroom/Scene/Entity.h"

namespace Vroom
{
	Scene* ScriptComponent::getScene()
	{
		return m_Scene;
	}

	const Scene* ScriptComponent::getScene() const
	{
		return m_Scene;
	}

	Entity ScriptComponent::getEntity() const
	{
		return m_Scene->getEntity(m_Handle);
	}

	float ScriptComponent::getDeltaTime() const
	{
		return Application::Get().getDeltaTime();
	}
}