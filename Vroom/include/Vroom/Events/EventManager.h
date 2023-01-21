#pragma once

#include <unordered_map>
#include <unordered_set>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Vroom/Events/Event.h"

namespace Vroom::Event
{
	class EventManager
	{
	public:
		EventManager() = default;

		void check(sf::Event& sfmlEvent);

		void trigger(const sf::Keyboard::Key& key, bool value);
		void trigger(const sf::Mouse::Button& mouseButton, bool value);

		void createEvent(const std::string& name);

		void bindInput(const std::string& eventName, const sf::Keyboard::Key& key);
		void bindInput(const std::string& eventName, const sf::Mouse::Button& mouseButton);

		void unbindInput(const std::string& eventName, const sf::Keyboard::Key& key);
		void unbindInput(const std::string& eventName, const sf::Mouse::Button& mouseButton);

		void bindCallback(const std::string& eventName, const Callback& callback);

		bool getEventState(const std::string& eventName) const;

	private:
		std::unordered_map<std::string, Event> m_Events;

		std::unordered_map<sf::Keyboard::Key, std::unordered_set<std::string>> m_Keys;
		std::unordered_map<sf::Mouse::Button, std::unordered_set<std::string>> m_MouseButtons;

		std::unordered_map<sf::Keyboard::Key, bool> m_KeyStates;
		std::unordered_map<sf::Mouse::Button, bool> m_MouseStates;
	};
}