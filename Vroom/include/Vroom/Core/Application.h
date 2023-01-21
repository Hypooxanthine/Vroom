#pragma once

#include <unordered_map>
#include <list>

#include "Vroom/Core/Minimal.h"
#include "Vroom/Events/EventManager.h"

#include <SFML/Graphics.hpp>

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
		 * @brief Closes the Application.
		*/
		void close();

		/**
		 * @brief Gets the last frame time.
		 * @return The last frame time as float.
		*/
		inline float getDeltaTime() const { return m_DeltaTime; }

		inline sf::RenderWindow& getWindow() { return m_Window; }
		inline const sf::RenderWindow& getWindow() const { return m_Window; }

		/**
		 * @brief Sets the current scene to be rendered by the engine.
		 * The ownership of the scene pointer will be taken by the engine. Just create it with the new keyword and send it as it to the function. You MUST NOT delete it yourself.
		 * @param scene A pointer to the scene.
		*/
		void setScene(Scene* scene);

		/**
		 * @brief Creates an event. Keys, mouse buttons and callbacks can be bound to the event.
		 * @see bindEventInput and registerEventCallback.
		 * @param eventName The name of the event.
		*/
		void registerEvent(const std::string& eventName);

		/**
		 * @brief Binds a key to an event. When the key is pressed/released, all the callbacks related to the eventName will be triggered.
		 * @param eventName The event name.
		 * @param key The event associated key.
		*/
		void bindEventInput(const std::string& eventName, sf::Keyboard::Key key);

		/**
		 * @brief Binds a mouse button to an event. When the mouse button is pressed/released, all the callbacks related to the eventName will be triggered.
		 * @param eventName The event name.
		 * @param button The event associated mouse button.
		*/
		void bindEventInput(const std::string& eventName, sf::Mouse::Button button);

		/**
		 * @brief Registers a callback associated with an event.
		 * @param eventName The event name.
		 * @param cb The callback to call when the event is detected. It has to accept a boolean as a parameter for wether the button is pressed or not.
		*/
		void registerEventCallback(const std::string& eventName, const Event::Callback& cb);

		/**
		 * @brief Gets the state of an event.
		 * @param eventName The name of the event.
		 * @return True if the event is currently triggered, otherwise returns false.
		*/
		bool getEventState(const std::string& eventName) const;

	private: // Private methods
		void manageEvents();
		void update();
		void render();

		void updateDeltaTime();

	private: // Private members
		static Application* s_Instance;

		// Window
		unsigned int m_WindowWidth, m_WindowHeight;
		std::string m_WindowTitle;
		sf::RenderWindow m_Window;

		// Events
		sf::Event m_SFMLEvent;
		Event::EventManager m_EventManager;

		// Time managing
		float m_DeltaTime;
		sf::Clock m_DtClock;

		// Scenes
		std::unique_ptr<Scene> m_CurrentScene;
	};
}