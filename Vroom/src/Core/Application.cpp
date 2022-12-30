#include "Vroom/Core/Application.h"

#include "Vroom/Scene/Scene.h"

namespace Vroom
{
	Application* Application::s_Instance = nullptr;

	Application::Application(unsigned int width, unsigned int height, const std::string& title)
		: m_WindowWidth(width), m_WindowHeight(height), m_WindowTitle(title),
		m_SFMLEvent(sf::Event()),
		m_DeltaTime(0.f)
	{
		VR_ASSERT_MSG(s_Instance == nullptr, "Application class can't be instanciated twice.");

		s_Instance = this;

		Log::init();
	}

	Application::Application()
		: Application(300, 300, "Vroom application")
	{}

	Application::~Application()
	{
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::run()
	{
		VR_ASSERT_MSG(m_CurrentScene != nullptr, "Can't run the Application with no scene.");

		m_Window.create(sf::VideoMode(m_WindowWidth, m_WindowHeight), m_WindowTitle, sf::Style::Close |sf::Style::Titlebar);

		m_DtClock.restart();

		while (m_Window.isOpen())
		{
			manageEvents();
			update();
			render();
			updateDeltaTime();
		}
	}

	void Application::setScene(Scene* scene)
	{
		if (m_CurrentScene != nullptr)
			m_CurrentScene->onSceneUnloaded();

		m_CurrentScene = std::unique_ptr<Scene>(scene);
		m_CurrentScene->onSceneLoaded();
	}

	void Application::registerEvent(const std::string& eventName, sf::Keyboard::Key key)
	{
		VR_ASSERT_MSG(!m_KeyboardEvents.contains(key)
					  || std::find(m_KeyboardEvents.at(key).first.begin(), m_KeyboardEvents.at(key).first.end(), eventName) == m_KeyboardEvents.at(key).first.end(),
			eventName + " keyboard event already exists.");

		m_KeyboardEvents.try_emplace(key, std::pair<std::list<std::string>, bool>(std::list<std::string>(), false));
		m_KeyboardEvents[key].first.push_back(eventName);
	}

	void Application::registerEvent(const std::string& eventName, sf::Mouse::Button button)
	{
		VR_ASSERT_MSG(!m_MouseEvents.contains(button)
					  || std::find(m_MouseEvents.at(button).first.begin(), m_MouseEvents.at(button).first.end(), eventName) == m_MouseEvents.at(button).first.end(),
			eventName + " mouse event already exists.");

		m_MouseEvents.try_emplace(button, std::pair<std::list<std::string>, bool>(std::list<std::string>(), false));
		m_MouseEvents[button].first.push_back(eventName);
	}

	void Application::registerEventCallback(const std::string& eventName, const std::function<void(bool)>& cb)
	{
		m_EventCallbacks.try_emplace(eventName, std::list<std::function<void(bool)>>());
		m_EventCallbacks[eventName].push_back(cb);
	}

	void Application::manageEvents()
	{
		while (m_Window.pollEvent(m_SFMLEvent))
		{
			if (m_SFMLEvent.type == sf::Event::Closed)
			{
				m_Window.close();
			}
			else if (m_SFMLEvent.type == sf::Event::KeyPressed)
			{
				if (m_KeyboardEvents.contains(m_SFMLEvent.key.code) && m_KeyboardEvents.at(m_SFMLEvent.key.code).second == false)
				{
					m_KeyboardEvents.at(m_SFMLEvent.key.code).second = true;

					for (const auto& eventName : m_KeyboardEvents.at(m_SFMLEvent.key.code).first)
						callEventCallbacks(eventName, true);
				}

			}
			else if (m_SFMLEvent.type == sf::Event::KeyReleased)
			{
				if (m_KeyboardEvents.contains(m_SFMLEvent.key.code) && m_KeyboardEvents.at(m_SFMLEvent.key.code).second == true)
				{
					m_KeyboardEvents.at(m_SFMLEvent.key.code).second = false;

					for (const auto& eventName : m_KeyboardEvents.at(m_SFMLEvent.key.code).first)
						callEventCallbacks(eventName, false);
				}
			}
			else if (m_SFMLEvent.type == sf::Event::MouseButtonPressed)
			{
				if (m_MouseEvents.contains(m_SFMLEvent.mouseButton.button) && m_MouseEvents.at(m_SFMLEvent.mouseButton.button).second == false)
				{
					m_MouseEvents.at(m_SFMLEvent.mouseButton.button).second = true;

					for (const auto& eventName : m_MouseEvents.at(m_SFMLEvent.mouseButton.button).first)
						callEventCallbacks(eventName, true);
				}
			}
			else if (m_SFMLEvent.type == sf::Event::MouseButtonReleased)
			{
				if (m_MouseEvents.contains(m_SFMLEvent.mouseButton.button) && m_MouseEvents.at(m_SFMLEvent.mouseButton.button).second == true)
				{
					m_MouseEvents.at(m_SFMLEvent.mouseButton.button).second = false;

					for (const auto& eventName : m_MouseEvents.at(m_SFMLEvent.mouseButton.button).first)
						callEventCallbacks(eventName, false);
				}
			}
		}
	}

	void Application::update()
	{
		m_CurrentScene->update();
	}

	void Application::render()
	{
		m_Window.clear();

		m_CurrentScene->render();

		m_Window.display();
	}

	void Application::updateDeltaTime()
	{
		m_DeltaTime = m_DtClock.restart().asSeconds();
	}

	void Application::callEventCallbacks(const std::string& cbName, bool pressed)
	{
		if (!m_EventCallbacks.contains(cbName)) return;

		for (const auto& cb : m_EventCallbacks.at(cbName))
			cb(pressed);
	}
}
