#include "Vroom/Core/Application.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/GLFWEventsConverter.h"
#include "Vroom/Core/Window.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Core/GameLayer.h"
#include "Vroom/Scene/Scene.h"
#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

Application* Application::s_Instance = nullptr;

Application::Application(int argc, char** argv)
    : m_Window(nullptr), m_LastFrameTimePoint(std::chrono::high_resolution_clock::now())
{
    VRM_ASSERT_MSG(s_Instance == nullptr, "Application already exists.");
    s_Instance = this;

    Log::Init();
    GLFWEventsConverter::Init();

    VRM_ASSERT(initGLFW());
    m_Window = std::make_unique<Window>();
    VRM_ASSERT(m_Window->create("Vroom engine", 800, 600));

    glewExperimental = GL_TRUE;
    VRM_ASSERT(glewInit() == GLEW_OK);
    
    AssetManager::Init();

    Renderer::Init();
    Renderer::Get().setViewport({ 0, 0 }, { m_Window->getWidth(), m_Window->getHeight()});

    // Pushing the game layer and storing it in a pointer (GameLayer is a special layer that can always be accessed)
    /// @todo Make sure the game layer is never deleted.
    m_GameLayer = &pushLayer<GameLayer>();

    VRM_LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        it->end();
    m_LayerStack.clear(); // Layer destructors called before shutting down the rendering context.

    Renderer::Shutdown();
    AssetManager::Shutdown();
    m_Window.release();
    glfwTerminate();
}

bool Application::initGLFW()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    return true;
}

void Application::initLayers()
{
    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        it->init();
}

void Application::run()
{
    initLayers();

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

void Application::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto dt = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_LastFrameTimePoint).count()) / 1'000'000'000.f;
    m_LastFrameTimePoint = now;

    // Updating from top to bottom
    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        it->update(dt);

    m_Window->updateEvents();

    while (m_Window->hasPendingEvents())
    {
        Event e = m_Window->pollEvent();
        
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
            it->submitEvent(e);
    }

}

void Application::draw()
{
    for (Layer& layer : m_LayerStack)
        layer.render();

    m_Window->swapBuffers();
}

} // namespace vrm
