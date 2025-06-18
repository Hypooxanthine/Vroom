#include "VroomEditor/EditorLayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>
#include <fstream>
#include <future>

#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

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
  to_json(j, data);

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
#ifndef VRM_BUILD_SCRIPTS_CMD
  VRM_LOG_WARN("Aborted: build command is not set");
  return;
#endif

  static constexpr std::string_view cmd = VRM_BUILD_SCRIPTS_CMD;

  OS::RunInShell(std::string(cmd), true);
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
    e.handled = true; });

  m_TriggerManager.createTrigger("MoveForward")
    .bindInput(vrm::KeyCode::W);
  m_TriggerManager.bindPermanentCallback("MoveForward", [this] (bool triggered) { m_EditorCamera.addMoveForward(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveBackward")
    .bindInput(vrm::KeyCode::S);
  m_TriggerManager.bindPermanentCallback("MoveBackward", [this] (bool triggered) { m_EditorCamera.addMoveForward(-(triggered ? 1.f : -1.f)); });
  m_TriggerManager.createTrigger("MoveRight")
    .bindInput(vrm::KeyCode::D);
  m_TriggerManager.bindPermanentCallback("MoveRight", [this] (bool triggered) { m_EditorCamera.addMoveRight(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveLeft")
    .bindInput(vrm::KeyCode::A);
  m_TriggerManager.bindPermanentCallback("MoveLeft", [this] (bool triggered) { m_EditorCamera.addMoveRight(-(triggered ? 1.f : -1.f)); });
  m_TriggerManager.createTrigger("MoveUp")
    .bindInput(vrm::KeyCode::Space);
  m_TriggerManager.bindPermanentCallback("MoveUp", [this] (bool triggered) { m_EditorCamera.addMoveUp(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveDown")
    .bindInput(vrm::KeyCode::LeftShift);
  m_TriggerManager.bindPermanentCallback("MoveDown", [this] (bool triggered) { m_EditorCamera.addMoveUp(-(triggered ? 1.f : -1.f)); });

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
  if (viewportInfo.active)
  {
    app.getWindow().setCursorVisible(false);
    m_EditorCamera.onUpdate(dt);
  }
  else
    app.getWindow().setCursorVisible(true);

  if (viewportInfo.simulating && !viewportInfo.paused)
  {
    app.getGameLayer().setShouldUpdate(true);
  }
  else
  {
    app.getGameLayer().setShouldUpdate(false);
  }
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

  m_EditorCamera.setViewportSize(
    static_cast<float>(newWidth),
    static_cast<float>(newHeight)
  );
}

void EditorLayer::_loadScriptsRuntimeLibrary()
{
  VRM_LOG_INFO("Loading scripts");

  bool ok;
  if (g_scriptLibraryPath.size() > 0)
  {
    ok = m_scriptsRuntime.load(g_scriptLibraryPath);

    if (!ok)
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
