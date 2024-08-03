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
    : m_Window(nullptr), m_Renderer(nullptr), m_LastFrameTimePoint(std::chrono::high_resolution_clock::now())
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
    
    // Instanciating asset manager
    m_AssetManager = std::make_unique<AssetManager>();

    m_Renderer = std::make_unique<Renderer>();
    m_Renderer->setViewport({ 0, 0 }, { m_Window->getWidth(), m_Window->getHeight()});

    // Pushing the game layer
    auto gameLayerUniquePtr = std::make_unique<GameLayer>();
    // The game layer should NOT be deleted, since it is the main purpose of the application.
    /// @todo Make sure the game layer is never deleted.
    m_GameLayer = gameLayerUniquePtr.get();
    // We want to initialize the game layer in the run method, so that user can load a scene before the game starts.
    pushLayer(std::move(gameLayerUniquePtr), false);

    VRM_LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
    /// @todo End all layers.
    m_GameLayer->end();
    m_LayerStack.pop();
    m_Window.release();
    glfwTerminate();
    m_Renderer.release();
    m_AssetManager.release();
}

bool Application::initGLFW()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    return true;
}

void Application::run()
{
    m_GameLayer->init();

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

void Application::pushLayer(std::unique_ptr<Layer>&& layer, bool init)
{
    m_LayerStack.push(std::move(layer));
    if (init)
        m_LayerStack.top()->init();
}

void Application::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto dt = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_LastFrameTimePoint).count()) / 1'000'000'000.f;
    m_LastFrameTimePoint = now;

    /// @todo Update layers from top to bottom.
    m_GameLayer->update(dt);

    m_Window->updateEvents();
    while (m_Window->hasPendingEvents())
    {
        Event e = m_Window->pollEvent();
        
        /// @todo Submit the event to layers from top to bottom.
        m_GameLayer->submitEvent(e);
    }

}

void Application::draw()
{
    /// @todo Draw layers from bottom to top.
    m_GameLayer->render();

    m_Window->swapBuffers();
}

} // namespace vrm
