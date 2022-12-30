#include "Vroom/Scene/Entity.h"

namespace Vroom
{
	Entity::Entity(entt::entity id, Scene* scene)
	{
		m_Handle = id;
		m_Scene = scene;
	}
}