#include "Vroom/Events/EventManager.h"

#include "Vroom/Core/Assert.h"

namespace Vroom::Event
{
	void EventManager::check(sf::Event& sfmlEvent)
	{
		switch (sfmlEvent.type)
		{
		case sf::Event::KeyPressed:
			trigger(sfmlEvent.key.code, true);
			break;
		case sf::Event::KeyReleased:
			trigger(sfmlEvent.key.code, false);
			break;
		case sf::Event::MouseButtonPressed:
			trigger(sfmlEvent.mouseButton.button, true);
			break;
		case sf::Event::MouseButtonReleased:
			trigger(sfmlEvent.mouseButton.button, false);
			break;
		case sf::Event::LostFocus:
			for (auto& [name, event] : m_Events)
				event.notifyFocusLost();
			for (auto& [name, state] : m_KeyStates)
				state = false;
			break;
		default:
			break;
		}
	}

	void EventManager::trigger(const sf::Keyboard::Key& key, bool value)
	{
		if (!m_Keys.contains(key)) return;

		// We check if the key is already pressed or already released. This can happen when the window focus is lost and gained.
		if (value && m_KeyStates[key]) return;
		if (!value && !m_KeyStates[key]) return;

		m_KeyStates[key] = value;

		for (const auto& eventName : m_Keys[key])
			m_Events[eventName].trigger(value);
	}

	void EventManager::trigger(const sf::Mouse::Button& mouseButton, bool value)
	{
		if (!m_MouseButtons.contains(mouseButton)) return;

		// We check if the button is already pressed or already released. This can happen when the window focus is lost and gained.
		if (value && m_MouseStates[mouseButton]) return;
		if (!value && !m_MouseStates[mouseButton]) return;

		m_MouseStates[mouseButton] = value;

		for (const auto& eventName : m_MouseButtons[mouseButton])
			m_Events[eventName].trigger(value);
	}

	void EventManager::createEvent(const std::string& name)
	{
		VR_ASSERT_MSG(!m_Events.contains(name) && !m_Events.contains(name), "Event of name {} already exists.", name);

		// We create a new event entry : a name and its associated event.
		m_Events.insert(std::pair<std::string, Event>(name, Event()));
	}

	void EventManager::bindInput(const std::string& eventName, const sf::Keyboard::Key& key)
	{
		if (!m_Keys.contains(key))
		{
			m_Keys[key] = std::unordered_set<std::string>{ eventName };
			m_KeyStates[key] = false;
		}
		
		if (m_Keys[key].contains(eventName)) return;

		m_Keys[key].insert(eventName);
	}

	void EventManager::bindInput(const std::string& eventName, const sf::Mouse::Button& mouseButton)
	{
		if (!m_MouseButtons.contains(mouseButton))
		{
			m_MouseButtons[mouseButton] = std::unordered_set<std::string>{ eventName };
			m_MouseStates[mouseButton] = false;
		}

		if (m_MouseButtons[mouseButton].contains(eventName)) return;

		m_MouseButtons[mouseButton].insert(eventName);
	}

	void EventManager::unbindInput(const std::string& eventName, const sf::Keyboard::Key& key)
	{
		if (!m_Keys.contains(key) || !m_Keys[key].contains(eventName)) return;

		m_Keys[key].erase(eventName);
	}

	void EventManager::unbindInput(const std::string& eventName, const sf::Mouse::Button& mouseButton)
	{
		if (!m_MouseButtons.contains(mouseButton) || !m_MouseButtons[mouseButton].contains(eventName)) return;

		m_MouseButtons[mouseButton].erase(eventName);
	}

	void EventManager::bindCallback(const std::string& eventName, const Callback& callback)
	{
		VR_ASSERT_MSG(m_Events.contains(eventName), "Event {} does not exist.", eventName);

		m_Events[eventName].bindCallback(callback);
	}

	bool EventManager::getEventState(const std::string& eventName) const
	{
		VR_ASSERT_MSG(m_Events.contains(eventName), "There is no event named {}.", eventName);

		return m_Events.at(eventName).isTriggered();
	}

}