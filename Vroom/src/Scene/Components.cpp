#include "Vroom/Scene/Components.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Scene.h"
#include "Vroom/Scene/Entity.h"

namespace Vroom
{
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
}