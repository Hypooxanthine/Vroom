#include "Vroom/Core/Application.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Core/GameLayer.h"
#include "Vroom/Core/Profiler.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Core/Window.h"
#include "Vroom/Event/GLFWEventsConverter.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Scene.h"

using namespace vrm;

Application* Application::s_Instance = nullptr;

Application::Application(int argc, char** argv)
  : m_Window(nullptr)
  , m_LastFrameTimePoint(std::chrono::high_resolution_clock::now())
{
  VRM_ASSERT_MSG(s_Instance == nullptr, "Application already exists.");
  s_Instance = this;

  Profiler::Init();

  Log::Init();
  GLFWEventsConverter::Init();
  VRM_ASSERT(initGLFW());
  m_Window = std::make_unique<Window>();
  VRM_ASSERT(m_Window->create("Vroom engine", 800, 600));

  glewExperimental = GL_TRUE;
  auto glewStatus  = glewInit();
  VRM_ASSERT_MSG(glewStatus == GLEW_OK,
                 "glewInit() failed with error code {}. Message: {}",
                 glewStatus, (const char*)(glewGetErrorString(glewStatus)));

  AssetManager::Init();

  // Pushing the game layer and storing it in a pointer (GameLayer is a special
  // layer that can always be accessed)
  /// @todo Make sure the game layer is never deleted.
  m_GameLayer = &pushLayer<GameLayer>();

  VRM_LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
  for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    it->end();
  m_LayerStack.clear(); // Layer destructors called before shutting down the
                        // rendering context.

  AssetManager::Shutdown();
  m_Window->destroy();
  m_Window.reset();

  glfwTerminate();

  Profiler::Shutdown();

  s_Instance = nullptr;
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
  for (Layer& layer : m_LayerStack) layer.init();
}

void Application::run()
{
  initLayers();

  // Main loop
  while (!m_PendingKilled)
  {
    VRM_PROFILE_SCOPE("Main loop");

    newFrame();
    update();
    draw();
    endFrame();
  }
}

void Application::exit() { m_PendingKilled = true; }

void Application::newFrame()
{
  VRM_PROFILE_SCOPE("Application::newFrame");

  m_DeltaTime.next();

  // Notifying new frame to all layers
  for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    it->newFrame();
}

void Application::update()
{
  VRM_PROFILE_SCOPE("Application::update");

  // Updating from top to bottom
  for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    it->update(m_DeltaTime);

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
  VRM_PROFILE_SCOPE("Application::draw");

  for (Layer& layer : m_LayerStack) layer.render();
}

void Application::endFrame()
{
  VRM_PROFILE_SCOPE("Application::endFrame");

  // Notifying last frame to all layers
  for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    it->endFrame();

  m_Window->swapBuffers();
}

void Application::setFrameRateLimit(uint16_t framerate)
{
  m_frameRateLimit = framerate;
  m_minFrameTimeNanoseconds =
    static_cast<uint64_t>(1.0e9 / static_cast<double>(framerate));
  m_timeSinceLastFrame = 0.f;
}

Scene& Application::getMainScene() { return getGameLayer().getScene(); }

Renderer& Application::getMainSceneRenderer()
{
  return getMainScene().getRenderer();
}
