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
		VR_ASSERT_MSG(!m_Window.isOpen(), "Application is already running.");
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

	void Application::registerEvent(const std::string& eventName)
	{
		m_EventManager.createEvent(eventName);
	}

	void Application::bindEventInput(const std::string& eventName, sf::Keyboard::Key key)
	{
		m_EventManager.bindInput(eventName, key);
	}

	void Application::bindEventInput(const std::string& eventName, sf::Mouse::Button button)
	{
		m_EventManager.bindInput(eventName, button);
	}

	void Application::registerEventCallback(const std::string& eventName, const Event::Callback& cb)
	{
		m_EventManager.bindCallback(eventName, cb);
	}

	void Application::manageEvents()
	{
		while (m_Window.pollEvent(m_SFMLEvent))
		{
			if (m_SFMLEvent.type == sf::Event::Closed)
				m_Window.close();

			m_EventManager.check(m_SFMLEvent);
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
}
