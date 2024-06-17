#include "Vroom/Core/Application.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/GLFWEventsConverter.h"

namespace vrm
{

Application::Application(int argc, char** argv)
{
    Log::Init();
    GLFWEventsConverter::Init();

    VRM_ASSERT(initGLFW());
    m_Window = std::make_unique<Window>();
    VRM_ASSERT(m_Window->create("Vroom engine", 600, 400));

    glewExperimental = TRUE;
    VRM_ASSERT(glewInit() == GLEW_OK);

    m_Renderer = std::make_unique<Renderer>();
    m_Renderer->setViewport({ 0, 0 }, { 600, 400 });

    LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
    m_Window.release();
    glfwTerminate();
    m_Renderer.release();
}

bool Application::initGLFW()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    return true;
}

void Application::run()
{
    while (!m_PendingKilled)
    {
        update();
        draw();
    }
}

void Application::exit()
{
    m_PendingKilled = true;
}

TriggerBinder Application::createTrigger(const std::string& name)
{
    return m_TriggerManager.createTrigger(name);
}

CustomEventBinder Application::createCustomEvent(const std::string &name)
{
    return m_CustomEventManager.createCustomEvent(name);
}

void Application::update()
{
    m_Window->updateEvents();
    while (m_Window->hasPendingEvents())
    {
        Event e = m_Window->pollEvent();

        std::string eventStr = "Unknown event";

        switch(e.type)
        {
        case Event::Type::KeyPressed:
            eventStr = "KeyPressed event (" + std::to_string(e.keyCode) + ")";
            break;
        case Event::Type::KeyReleased:
            eventStr = "KeyReleased event (" + std::to_string(e.keyCode) + ")";
            break;
        case Event::Type::MousePressed:
            eventStr = "MousePressed event (" + std::to_string(e.mouseCode) + ")";
            break;
        case Event::Type::MouseReleased:
            eventStr = "MouseReleased event (" + std::to_string(e.mouseCode) + ")";
            break;
        case Event::Type::Scroll:
            eventStr = "Scroll event (" + std::to_string(e.scrollY) + ")";
            break;
        case Event::Type::GainedFocus:
            eventStr = "GainedFocus event";
            break;
        case Event::Type::LostFocus:
            eventStr = "LostFocus event";
            break;
        case Event::Type::WindowsResized:
            eventStr = "Resize event (" + std::to_string(e.newWidth) + ", " + std::to_string(e.newHeight) + ")";
            m_Renderer->setViewport({ 0.f, 0.f }, { static_cast<float>(e.newWidth), static_cast<float>(e.newHeight) });
            break;
        case Event::Type::Exit:
            eventStr = "Exit event";
            exit();
            break;
        }

        LOG_TRACE("{} triggered.", eventStr);

        m_TriggerManager.check(e);
        m_CustomEventManager.check(e);
    }
}

void Application::draw()
{
    m_Renderer->beginScene();

    m_Renderer->endScene();
    m_Window->swapBuffers();
}

} // namespace vrm
