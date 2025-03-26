#include "Vroom/Core/GameLayer.h"

#include "Vroom/Core/Application.h"
#include "Vroom/Scene/Scene.h"
#include "Vroom/Event/Event.h"

#include "Vroom/Render/Renderer.h"

using namespace vrm;

GameLayer::GameLayer()
    : m_CurrentScene(nullptr), m_NextScene(nullptr)
{
  Renderer &renderer = Renderer::Get();

  m_FrameBuffer.create({.onScreen = true,
                        .width = static_cast<int>(renderer.getViewportSize().x),
                        .height = static_cast<int>(renderer.getViewportSize().y),
                        .useBlending = true,
                        .useDepthTest = true,
                        .clearColor = {0.1f, 0.1f, 0.1f, 1.f}});

  m_CustomEventManager.createCustomEvent("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE")
      .bindInput(Event::Type::WindowsResized);

  m_CustomEventManager
      .bindPermanentCallback("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE", [&renderer](const vrm::Event &e)
                             { renderer.setViewport({0.f, 0.f}, {static_cast<float>(e.newWidth), static_cast<float>(e.newHeight)}); })
      .bindPermanentCallback("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE", [this](const vrm::Event &e)
                             { this->m_FrameBuffer.resize(e.newWidth, e.newHeight); })
  ;
}

GameLayer::~GameLayer()
{
}

void GameLayer::onInit()
{
  VRM_DEBUG_ASSERT_MSG(m_NextScene != nullptr, "Make sure you loaded a scene before running the application.");
  m_CurrentScene = std::move(m_NextScene);
  m_CurrentScene->init();
}

void GameLayer::onEnd()
{
  m_CurrentScene->end();
}

void GameLayer::onNewFrame()
{
  checkNextScene();
}

void GameLayer::onUpdate(float dt)
{
  m_CurrentScene->update(dt);
}

void GameLayer::onRender()
{
  m_CurrentScene->render();
}

void GameLayer::onEvent(Event &e)
{
  m_TriggerManager.check(e);
  m_CustomEventManager.check(e);
}

TriggerBinder GameLayer::createTrigger(const std::string &triggerName)
{
  return m_TriggerManager.createTrigger(triggerName);
}

TriggerBinder GameLayer::getTrigger(const std::string &triggerName)
{
  return m_TriggerManager.getBinder(triggerName);
}

CustomEventBinder GameLayer::createCustomEvent(const std::string &customEventName)
{
  return m_CustomEventManager.createCustomEvent(customEventName);
}

CustomEventBinder GameLayer::getCustomEvent(const std::string &customEventName)
{
  return m_CustomEventManager.getBinder(customEventName);
}

void GameLayer::checkNextScene()
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
  m_CurrentScene->init();
}

void GameLayer::loadScene_Internal(std::unique_ptr<Scene> &&scene)
{
  m_NextScene = std::move(scene);
}
