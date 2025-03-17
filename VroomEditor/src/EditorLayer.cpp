#include "VroomEditor/EditorLayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "VroomEditor/EditorScene.h"
#include "VroomEditor/TestScene/TestScene.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"

using namespace vrm;

EditorLayer::EditorLayer()
    : m_MainMenuBar(),
      m_StatisticsPanel(),
      m_Viewport(),
      m_AssetBrowser(),
      m_Font(nullptr),
      m_FrameAccumulator(0),
      m_TimeAccumulator(0.f),
      m_TimeSample(1.f),
      m_EditorCamera(0.1f, 100.f, glm::radians(90.f), 0.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f))
{
  // We need to load a first scene before initialization of layers, because game layer will be initialized first.
  // Just loading the default scene.
  loadScene<Scene>();
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::loadScene(std::unique_ptr<Scene> &&scene)
{
  VRM_LOG_INFO("Opening scene...");
  scene->setCamera(&m_EditorCamera);
  auto &gameLayer = Application::Get().getGameLayer();
  gameLayer.loadScene(std::move(scene));
}

void EditorLayer::onInit()
{
  // Events setup
  m_CustomEventManager.createCustomEvent("Exit")
      .bindInput(Event::Type::Exit)
      .bindInput(Event::Type::KeyPressed, vrm::KeyCode::Escape);

  m_CustomEventManager.bindPermanentCallback("Exit", [](const Event &e)
                                             { Application::Get().exit(); });

  // Engine setup
  auto &app = Application::Get();
  app.getGameLayer().getFrameBuffer().setOnScreenRender(false);
  app.getGameLayer().setShouldHandleEvents(false);
  app.getGameLayer().setShouldUpdate(false);
  app.getGameLayer().setShouldRender(true);

  // Frame buffer
  FrameBuffer::Specification specs = {
      .onScreen = true,
      .width = app.getWindow().getWidth(),
      .height = app.getWindow().getHeight(),
      .useBlending = true,
      .useDepthTest = true,
      .clearColor = {0.1f, 0.1f, 0.1f, 1.0f}};
  m_FrameBuffer.create(specs);
  m_FrameBuffer.bind();

  // Imgui setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplGlfw_InitForOpenGL(Application::Get().getWindow().getGLFWHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 450");

  m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
  VRM_ASSERT_MSG(m_Font, "Failed to load font.");

  // UI setup
  m_Viewport.setFrameBuffer(&app.getGameLayer().getFrameBuffer());

  // Events
  m_CustomEventManager.createCustomEvent("EditorCameraRotation")
      .bindInput(Event::Type::MouseMoved);
  m_CustomEventManager.bindPermanentCallback("EditorCameraRotation", [this](const Event &e)
                                             {
          m_EditorCamera.submitLookRight(static_cast<float>(e.mouseDeltaX));
          m_EditorCamera.submitLookUp(static_cast<float>(-e.mouseDeltaY));
          e.handled = true; });

  m_TriggerManager.createTrigger("MoveForward")
      .bindInput(vrm::KeyCode::W);
  m_TriggerManager.bindPermanentCallback("MoveForward", [this](bool triggered)
                                         { m_EditorCamera.addMoveForward(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveBackward")
      .bindInput(vrm::KeyCode::S);
  m_TriggerManager.bindPermanentCallback("MoveBackward", [this](bool triggered)
                                         { m_EditorCamera.addMoveForward(-(triggered ? 1.f : -1.f)); });
  m_TriggerManager.createTrigger("MoveRight")
      .bindInput(vrm::KeyCode::D);
  m_TriggerManager.bindPermanentCallback("MoveRight", [this](bool triggered)
                                         { m_EditorCamera.addMoveRight(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveLeft")
      .bindInput(vrm::KeyCode::A);
  m_TriggerManager.bindPermanentCallback("MoveLeft", [this](bool triggered)
                                         { m_EditorCamera.addMoveRight(-(triggered ? 1.f : -1.f)); });
  m_TriggerManager.createTrigger("MoveUp")
      .bindInput(vrm::KeyCode::Space);
  m_TriggerManager.bindPermanentCallback("MoveUp", [this](bool triggered)
                                         { m_EditorCamera.addMoveUp(triggered ? 1.f : -1.f); });
  m_TriggerManager.createTrigger("MoveDown")
      .bindInput(vrm::KeyCode::LeftShift);
  m_TriggerManager.bindPermanentCallback("MoveDown", [this](bool triggered)
                                         { m_EditorCamera.addMoveUp(-(triggered ? 1.f : -1.f)); });
}

void EditorLayer::onEnd()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void EditorLayer::onUpdate(float dt)
{
  auto &app = Application::Get();

  // Frame time management
  m_FrameAccumulator++;
  m_TimeAccumulator += dt;
  if (m_TimeAccumulator >= m_TimeSample)
  {
    m_StatisticsPanel.frameTime = m_TimeSample / m_FrameAccumulator;
    m_FrameAccumulator = 0;
    m_TimeAccumulator -= m_TimeSample;
  }

  // Handling viewport resize
  if (m_Viewport.didSizeChangeLastFrame())
    onViewportResize();

  // If the viewport is active, we update the editor camera
  if (m_Viewport.isActive())
  {
    app.getWindow().setCursorVisible(false);
    m_EditorCamera.onUpdate(dt);
  }
  else
    app.getWindow().setCursorVisible(true);

  if (m_Viewport.isSimulating() && !m_Viewport.isPaused())
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
  m_FrameBuffer.bind();
  m_FrameBuffer.clearColorBuffer();

  onImgui();
}

void EditorLayer::onEvent(vrm::Event &e)
{
  m_CustomEventManager.check(e);
  m_TriggerManager.check(e);
}

void EditorLayer::onImgui()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::PushFont(m_Font);

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  m_MainMenuBar.renderImgui();
  m_StatisticsPanel.renderImgui();
  m_Viewport.renderImgui();

  if (m_AssetBrowser.renderImgui())
  {
    auto action = m_AssetBrowser.getAction();

    switch (action)
    {
    case AssetElement::EAction::eLoadScene:
    {
      auto scene = std::make_unique<Scene>();
      if (scene->loadFromAsset(AssetManager::Get().getAsset<SceneAsset>(m_AssetBrowser.getSelectedAsset().string())))
        loadScene(std::move(scene));
      else
        VRM_LOG_ERROR("Failed to load scene.");
    }
    break;

    default:
      break;
    }
  }

  if (ImGui::Begin("Tests"))
  {
    if (ImGui::Button("Load default scene"))
    {
      loadScene<Scene>();
    }

    if (ImGui::Button("Load TestScene"))
    {
      loadScene<TestScene>();
    }
  }
  ImGui::End();

  ImGui::PopFont();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorLayer::onViewportResize()
{
  Event e;
  e.type = Event::Type::WindowsResized;
  e.newWidth = static_cast<int>(m_Viewport.getLastViewportSize().x);
  e.newHeight = static_cast<int>(m_Viewport.getLastViewportSize().y);
  auto &gameLayer = Application::Get().getGameLayer();

  // We trick the game layer to handle resize event even if it is not handling events
  // because we want the viewport to be smooth on resize, even when the game isn't playing.
  bool handledEvents = gameLayer.isHandlingEvents();
  gameLayer.setShouldHandleEvents(true);
  gameLayer.submitEvent(e);
  gameLayer.setShouldHandleEvents(handledEvents);

  m_EditorCamera.setViewportSize(m_Viewport.getLastViewportSize().x, m_Viewport.getLastViewportSize().y);
}
