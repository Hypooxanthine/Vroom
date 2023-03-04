#include "Vroom/Scene/Components.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Scene.h"
#include "Vroom/Scene/Entity.h"

#include "Vroom/Scene/ChunkManager.h"

namespace Vroom
{
	void TransformComponent::notifyMove()
	{
		if (getScene().m_RenderChunks == nullptr) return;
		getScene().m_RenderChunks->refreshEntity(getEntity());
	}

	float ScriptComponent::getDeltaTime() const
	{
		return Application::Get().getDeltaTime();
	}
	bool ScriptComponent::getEventState(const std::string& eventName) const
	{
		return Application::Get().getEventState(eventName);
	}

	void ScriptComponent::quitGame() const
	{
		Application::Get().close();
	}

	CameraComponent::CameraComponent()
		: TransformComponent()
	{
		const auto windowSize = (sf::Vector2f)Application::Get().getWindowSize();

		m_View = sf::View
		(
			sf::Vector2f((windowSize.x / 2.f), (windowSize.y / 2.f)),
			sf::Vector2f((windowSize.x), (windowSize.y))
		);
	}

	void CameraComponent::setSize(const sf::Vector2f& size)
	{
		m_View.setSize((size.x), (size.y));
	}

	void CameraComponent::setSize(float x, float y)
	{
		setSize({ x, y });
	}

	void CameraComponent::activate()
	{
		getScene().setCamera(*this);
	}

	void CameraComponent::updatePosition()
	{
		const auto& entityTranslation = getEntity().getComponent<TransformComponent>().getTranslation();

		m_View.setCenter((entityTranslation.x), (entityTranslation.y));
	}

	Entity Component::getEntity() const 
	{
		return m_Scene->getEntity(m_Handle);
	}

	void ColliderComponent::setPosition(const sf::Vector2f& position)
	{
		m_Rect.left = position.x;
		m_Rect.top = position.y;
	}

	void ColliderComponent::setSize(const sf::Vector2f& size)
	{
		m_Rect.width = size.x;
		m_Rect.height = size.y;
	}

	sf::Vector2f ColliderComponent::getPosition() const
	{
		return { m_Rect.left, m_Rect.top };
	}

	sf::Vector2f ColliderComponent::getSize() const
	{
		return { m_Rect.width, m_Rect.height };
	}

	void LayerComponent::setLayer(int l)
	{
		m_Layer = l;
		getScene().m_RenderChunks->refreshEntity(getEntity());
	}
}