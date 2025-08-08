#include "VroomEditor/EditorLayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Render/Renderer.h>
#include <Vroom/Asset/Parsing/SceneParsing.h>
#include <fstream>
#include <future>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"
#include "VroomEditor/UserInterface/SceneGraph.h"
#include "VroomEditor/UserInterface/Viewport.h"

#include "VroomEditor/EditorScene.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Tools/Os.h"

#ifdef VRM_RUNTIME_SCRIPTS_PATH
static constexpr std::string_view g_scriptLibraryPath = VRM_RUNTIME_SCRIPTS_PATH;
#else
static constexpr std::string_view g_scriptLibraryPath = "";
#endif

using namespace vrm;

static EditorLayer* INSTANCE = nullptr;

EditorLayer::EditorLayer() :
  m_EditorCamera(0.1f, 100.f, glm::radians(90.f), 0.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f))
{
  VRM_ASSERT_MSG(INSTANCE == nullptr, "Only one instance of EditorLayer is allowed");
  INSTANCE = this;
  // We need to load a first scene before initialization of layers, because game layer will be initialized first.
  // Just loading the default scene.
  unloadScene(true);
}

EditorLayer::~EditorLayer()
{
  INSTANCE = nullptr;
}

EditorLayer& EditorLayer::Get()
{
  return *INSTANCE;
}

void EditorLayer::loadScene(const std::string& name, std::unique_ptr<Scene>&& scene)
{
  scene->setCamera(&m_EditorCamera);
  auto& gameLayer = Application::Get().getGameLayer();
  gameLayer.loadScene(std::move(scene));
  m_loadedScene = name;
}

void EditorLayer::loadScene(const std::string& sceneAssetName)
{
  VRM_LOG_INFO("Loading scene...");
  auto scene = std::make_unique<Scene>();
  scene->loadFromAsset(AssetManager::Get().getAsset<SceneAsset>(sceneAssetName));
  loadScene(sceneAssetName, std::move(scene));
}

void EditorLayer::unloadScene(bool force)
{
  if (isSceneLoaded() || force)
  {
    loadScene<Scene>("");
  }
}

void EditorLayer::saveScene()
{
  if (!isSceneLoaded())
  {
    VRM_LOG_WARN("Could not save scene: no scene is loaded");
    return;
  }

  auto& gameLayer = Application::Get().getGameLayer();
  const Scene& scene = gameLayer.getScene();
  SceneData data = scene.getSceneData();

  json j;

  try
  {
    j = data;
  }
  catch (const nlohmann::json::exception& e)
  {
    VRM_LOG_ERROR("Error while serializing scene to json: {}", e.what());
    return;
  }

  {
    std::ofstream ofs;
    ofs.open(m_loadedScene, std::ios_base::trunc);

    ofs << j;
  }

  AssetManager::Get().reloadAsset<SceneAsset>(m_loadedScene);
}

void EditorLayer::importFile(const std::filesystem::path& file)
{
  
}

void EditorLayer::buildScripts()
{
  VRM_LOG_WARN("Not implemented");
  return;
#ifndef VRM_SCRIPT_BUILDER_CMD
  VRM_LOG_WARN("Aborted: build command is not set");
  return;
#else
  std::string cmd = VRM_SCRIPT_BUILDER_CMD;

  VRM_LOG_INFO("Running command {}", cmd);
  OS::Run(cmd, true);
#endif
}

void EditorLayer::reloadScripts()
{
  VRM_LOG_WARN("Not implemented");
}

void EditorLayer::onInit()
{
  // Engine setup
  auto& app = Application::Get();
  app.getGameLayer().setShouldHandleEvents(false);
  app.getGameLayer().setShouldUpdate(false);
  app.getGameLayer().setShouldRender(true);

  // Events setup
  m_CustomEventManager.createCustomEvent("Exit")
    .bindInput(Event::Type::Exit)
    ;

  m_CustomEventManager.bindPermanentCallback("Exit", [] (const Event& e) { Application::Get().exit(); });

  // Events
  m_CustomEventManager.createCustomEvent("EditorCameraRotation")
    .bindInput(Event::Type::MouseMoved);
  m_CustomEventManager.bindPermanentCallback("EditorCameraRotation", [this] (const Event& e) {
    m_EditorCamera.submitLookRight(static_cast<float>(e.mouseDeltaX));
    m_EditorCamera.submitLookUp(static_cast<float>(-e.mouseDeltaY));
    e.handled = true;
  });

  m_TriggerManager.createTrigger("MoveForward")
    .bindInput(vrm::KeyCode::W);
  m_TriggerManager.bindPermanentCallback("MoveForward", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveForward(triggered ? 1.f : -1.f);
  });

  m_TriggerManager.createTrigger("MoveBackward")
    .bindInput(vrm::KeyCode::S);
  m_TriggerManager.bindPermanentCallback("MoveBackward", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveForward(-(triggered ? 1.f : -1.f));
  });

  m_TriggerManager.createTrigger("MoveRight")
    .bindInput(vrm::KeyCode::D);
  m_TriggerManager.bindPermanentCallback("MoveRight", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveRight(triggered ? 1.f : -1.f);
  });

  m_TriggerManager.createTrigger("MoveLeft")
    .bindInput(vrm::KeyCode::A);
  m_TriggerManager.bindPermanentCallback("MoveLeft", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveRight(-(triggered ? 1.f : -1.f));
  });

  m_TriggerManager.createTrigger("MoveUp")
    .bindInput(vrm::KeyCode::Space);
  m_TriggerManager.bindPermanentCallback("MoveUp", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveUp(triggered ? 1.f : -1.f);
  });

  m_TriggerManager.createTrigger("MoveDown")
    .bindInput(vrm::KeyCode::LeftShift);
  m_TriggerManager.bindPermanentCallback("MoveDown", [this] (bool triggered) {
    if (triggered) UserInterfaceLayer::Get().getViewport().allowActivation();
    m_EditorCamera.addMoveUp(-(triggered ? 1.f : -1.f));
  });

  // Scripts
  _loadScriptsRuntimeLibrary();
}

void EditorLayer::onEnd()
{
}

void EditorLayer::onUpdate(const DeltaTime& dt)
{
  auto& app = Application::Get();

  const auto& viewportInfo = UserInterfaceLayer::Get().getViewportInfo();

  if (viewportInfo.justChangedSize)
    onViewportResize(viewportInfo.width, viewportInfo.height);

  // If the viewport is active, we update the editor camera
  const bool cameraInputs = viewportInfo.active;
    
  if (cameraInputs)
  {
    m_EditorCamera.onUpdate(dt);
  }
  else
  {
    // m_EditorCamera.clearInputs();
  }

  const bool cursorVisible = !viewportInfo.active;
  app.getWindow().setCursorVisible(cursorVisible);

  const bool simul = viewportInfo.simulating;
  const bool lastSimul = m_lastViewportInfos.simulating;

  const bool playing = viewportInfo.playing;
  const bool lastPlaying = m_lastViewportInfos.playing;

  const bool updt = (simul || playing) && !viewportInfo.paused;
  const bool lastUpdt = (lastSimul || lastPlaying) && !m_lastViewportInfos.paused;

  if (updt != lastUpdt)
  {
    app.getGameLayer().setShouldUpdate(updt);
  }

  if (playing != lastPlaying)
  {
    if (playing)
    {
      Application::Get().getGameLayer().getScene().spawn();
    }
    else
    {
      // Reloading scene
      if (isSceneLoaded())
        loadScene(m_loadedScene);
    }
  }

  if (simul != lastSimul)
  {
    if (simul)
    {
      // When simulating started
    }
    else
    {
      // Reloading scene
      if (isSceneLoaded())
        loadScene(m_loadedScene);
    }
  }

  m_lastViewportInfos = viewportInfo;
}

void EditorLayer::onRender()
{
}

void EditorLayer::onEvent(vrm::Event& e)
{
  m_CustomEventManager.check(e);
  m_TriggerManager.check(e);
}

void EditorLayer::onViewportResize(int newWidth, int newHeight)
{
  Event e;
  e.type = Event::Type::WindowsResized;
  e.newWidth = newWidth;
  e.newHeight = newHeight;
  auto& gameLayer = Application::Get().getGameLayer();

  // We trick the game layer to handle resize event even if it is not handling events
  // because we want the viewport to be smooth on resize, even when the game isn't playing.
  bool handledEvents = gameLayer.isHandlingEvents();
  gameLayer.setShouldHandleEvents(true);
  gameLayer.submitEvent(e);
  gameLayer.setShouldHandleEvents(handledEvents);
}

void EditorLayer::_loadScriptsRuntimeLibrary()
{
  VRM_LOG_INFO("Loading scripts");

  bool ok;
  if (g_scriptLibraryPath.size() > 0)
  {
    if (!(ok = m_scriptsRuntime.load(g_scriptLibraryPath)))
    {
      VRM_LOG_ERROR("Error while loading scripts runtime library {}", g_scriptLibraryPath);
    }
  }
  else
  {
    VRM_LOG_WARN("No scripts runtime library path");
    ok = false;
  }

  if (ok)
  {
    VRM_LOG_INFO("Scripts runtime library {} loaded", g_scriptLibraryPath.data());
  }
}
