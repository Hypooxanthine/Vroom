#pragma once

#include <unordered_map>
#include <list>

#include "Vroom/Core/Minimal.h"

#include "SFML/Graphics.hpp"

namespace Vroom
{
	class Scene;

	class Application
	{
	public: // Public methods

		/**
		 * @brief Creates an Application and its window with asked width, height and title.
		 * @param width Window width.
		 * @param height Window height.
		 * @param title Window title.
		*/
		Application(unsigned int width, unsigned int height, const std::string& title);

		/**
		 * @brief Creates an Application with predefined window width, height and title.
		*/
		Application();

		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;

		/**
		 * @brief Gets the Application singleton instance.
		 * @return A reference to the Application instance.
		*/
		static Application& Get();

		/**
		 * @brief Runs the Application.
		*/
		void run();

		/**
		 * @brief Gets the last frame time.
		 * @return The last frame time as float.
		*/
		inline float getDeltaTime() const { return m_DeltaTime; }

		inline sf::RenderWindow& getWindow() { return m_Window; }
		inline const sf::RenderWindow& getWindow() const { return m_Window; }

		void setScene(Scene* scene);

		/**
		 * @brief Registers a keyboard event. When the key is pressed/released, all the callbacks related to the eventName will be triggered.
		 * @param eventName The event name.
		 * @param key The event associated key.
		*/
		void registerEvent(const std::string& eventName, sf::Keyboard::Key key);

		/**
		 * @brief Registers a mouse event. When the mouse button is pressed/released, all the callbacks related to the eventName will be triggered.
		 * @param eventName The event name.
		 * @param button The event associated mouse button.
		*/
		void registerEvent(const std::string& eventName, sf::Mouse::Button button);

		/**
		 * @brief Registers a callback associated with an event.
		 * @param eventName The event name.
		 * @param cb The callback to call when the event is detected. It has to accept a boolean as a parameter for wether the button is pressed or not.
		*/
		void registerEventCallback(const std::string& eventName, const std::function<void(bool)>& cb);

	private: // Private methods
		void manageEvents();
		void update();
		void render();

		void updateDeltaTime();

		void callEventCallbacks(const std::string& cbName, bool pressed);

	private: // Private members
		static Application* s_Instance;

		// Window
		unsigned int m_WindowWidth, m_WindowHeight;
		std::string m_WindowTitle;
		sf::RenderWindow m_Window;

		// Events
		sf::Event m_SFMLEvent;
		std::unordered_map<sf::Keyboard::Key, std::pair<std::list<std::string>, bool>> m_KeyboardEvents;
		std::unordered_map<sf::Mouse::Button, std::pair<std::list<std::string>, bool >> m_MouseEvents;

		std::unordered_map<std::string, std::list<std::function<void(bool)>>> m_EventCallbacks;

		// Time managing
		float m_DeltaTime;
		sf::Clock m_DtClock;

		// Scenes
		std::unique_ptr<Scene> m_CurrentScene;
	};
}