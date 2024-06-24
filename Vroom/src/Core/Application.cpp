#include "Vroom/Core/Application.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/GLFWEventsConverter.h"
#include "Vroom/Core/Window.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Scene.h"

namespace vrm
{

Application::Application(int argc, char** argv)
    : m_Window(nullptr), m_Renderer(nullptr), m_CurrentScene(nullptr), m_LastFrameTimePoint(std::chrono::high_resolution_clock::now())
{
    Log::Init();
    GLFWEventsConverter::Init();

    VRM_ASSERT(initGLFW());
    m_Window = std::make_unique<Window>();
    VRM_ASSERT(m_Window->create("Vroom engine", 800, 600));

    glewExperimental = TRUE;
    VRM_ASSERT(glewInit() == GLEW_OK);

    m_Renderer = std::make_unique<Renderer>();
    m_Renderer->setViewport({ 0, 0 }, { m_Window->getWidth(), m_Window->getHeight()});

    LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
    m_CurrentScene.release();
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
    VRM_DEBUG_ASSERT_MSG(m_NextScene != nullptr, "Make sure you loaded a scene before running the application.");

    m_CurrentScene = std::move(m_NextScene);
    m_CurrentScene->init(this);

    while (!m_PendingKilled)
    {
        checkNextScene();
        update();
        draw();
    }

    m_CurrentScene->end();
}

void Application::exit()
{
    m_PendingKilled = true;
}

TriggerBinder Application::createTrigger(const std::string& triggerName)
{
    return m_TriggerManager.createTrigger(triggerName);
}

TriggerBinder Application::getTrigger(const std::string& triggerName)
{
    return m_TriggerManager.getBinder(triggerName);
}

CustomEventBinder Application::createCustomEvent(const std::string &customEventName)
{
    return m_CustomEventManager.createCustomEvent(customEventName);
}

CustomEventBinder Application::getCustomEvent(const std::string& customEventName)
{
    return m_CustomEventManager.getBinder(customEventName);
}

void Application::checkNextScene()
{
    // If nothing to load, just return
    if (m_NextScene == nullptr)
        return;

    // There is a scene to load
    // We are sure we that a scene is already loaded,
    // because first scene detection is handled in loadScene_Impl.
    // So we don't need to say "If currentScene exists, then currentScene.end()", we can juste end it.

    m_CurrentScene->end();
    m_CurrentScene = std::move(m_NextScene);
    m_CurrentScene->init(this);
}

void Application::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto dt = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_LastFrameTimePoint).count()) / 1'000'000'000.f;
    m_LastFrameTimePoint = now;

    m_CurrentScene->update(dt);

    m_Window->updateEvents();
    while (m_Window->hasPendingEvents())
    {
        Event e = m_Window->pollEvent();

        m_TriggerManager.check(e);
        m_CustomEventManager.check(e);
    }

}

void Application::draw()
{
    m_CurrentScene->render();
    m_Window->swapBuffers();
}

void Application::loadScene_Internal(std::unique_ptr<Scene>&& scene)
{
    m_NextScene = std::move(scene);
}

} // namespace vrm
