#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Render/Renderer.h>

#include "VroomEditor/EditorLayer.h"

#include "VroomEditor/UserInterface/MainMenuBar.h"
#include "VroomEditor/UserInterface/StatisticsPanel.h"
#include "VroomEditor/UserInterface/RenderSettingsPanel.h"
#include "VroomEditor/UserInterface/Viewport.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"
#include "VroomEditor/UserInterface/SceneGraph.h"

using namespace vrm;

static UserInterfaceLayer* INSTANCE;

UserInterfaceLayer::UserInterfaceLayer() :
    Layer()
  , m_Font(nullptr)
{
  VRM_ASSERT_MSG(INSTANCE == nullptr, "Only one instance of UserInterfaceLayer is allowed");
  INSTANCE = this;
}

UserInterfaceLayer::~UserInterfaceLayer()
{

}

UserInterfaceLayer& UserInterfaceLayer::Get()
{
  return *INSTANCE;
}

void UserInterfaceLayer::onInit()
{
  auto &app = Application::Get();

  VRM_LOG_INFO("UserInterfaceLayer init");

  // Imgui setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplGlfw_InitForOpenGL(Application::Get().getWindow().getGLFWHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 450");

  m_Font = io.Fonts->AddFontFromFileTTF("Resources/Editor/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
  VRM_ASSERT_MSG(m_Font, "Failed to load font.");

  // Interfaces setup
  auto& mainMenuBar = emplaceImGuiElement<MainMenuBar>();
  auto& statisticsPanel = emplaceImGuiElement<StatisticsPanel>();
  auto& renderSettingsPanel = emplaceImGuiElement<RenderSettingsPanel>();
  auto& viewport = emplaceImGuiElement<Viewport>();
  auto& assetBrowser = emplaceImGuiElement<AssetBrowser>();
  auto& sceneGraph = emplaceImGuiElement<SceneGraph>();

  viewport.setRenderTexture(&Renderer::Get().getMainFrameBuffer().getColorAttachmentTexture(0));

  m_CustomEventManager.createCustomEvent("OSFileDrop").bindInput(Event::Type::FileDrop);
  m_CustomEventManager.bindPermanentCallback("OSFileDrop", [this](const Event& e) { this->fileDropCallback(e); e.handled = true; });

  m_CustomEventManager.createCustomEvent("OSDragEnter").bindInput(Event::Type::FileDragEnter);
  m_CustomEventManager.bindPermanentCallback("OSDragEnter", [this](const Event& e) { this->m_isDraggingOSFile = true; e.handled = true; });

  m_CustomEventManager.createCustomEvent("OSDragLeave").bindInput(Event::Type::FileDragLeave);
  m_CustomEventManager.bindPermanentCallback("OSDragLeave", [this](const Event& e) { this->m_isDraggingOSFile = false; e.handled = true; });
}

void UserInterfaceLayer::onEnd()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UserInterfaceLayer::onUpdate(const DeltaTime& dt)
{
  m_fileDrop.clear();
}

void UserInterfaceLayer::onRender()
{
  gl::FrameBuffer::GetDefaultFrameBuffer().bind();
  resetUIInfos();
  renderImgui();
}

void UserInterfaceLayer::onEvent(vrm::Event &e)
{
  m_CustomEventManager.check(e);
}

void UserInterfaceLayer::resetUIInfos()
{
  m_ViewportInfo.justChangedSize = false;
}

void UserInterfaceLayer::renderImgui()
{
  auto& editorLayer = EditorLayer::Get();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::PushFont(m_Font);

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  if (m_isDraggingOSFile || m_fileDrop.files)
  {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
    {
      if (m_fileDrop.files)
        ImGui::SetDragDropPayload("OSFileDrop", &m_fileDrop, sizeof(m_fileDrop));
      else
        ImGui::SetDragDropPayload("OSFileDrop", nullptr, 0);
      ImGui::EndDragDropSource();
    }
  }
  
  for (auto& element : m_Elements)
  {
    element->renderImgui();
  }

  ImGui::PopFont();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterfaceLayer::onViewportResize(int w, int h)
{
  m_ViewportInfo.justChangedSize = true;
  m_ViewportInfo.width = w;
  m_ViewportInfo.height = h;
}

void UserInterfaceLayer::onViewportActive(bool active)
{
  m_ViewportInfo.active = active;
}

void UserInterfaceLayer::onViewportSimulating(bool simulating)
{
  m_ViewportInfo.simulating = simulating;
}

void UserInterfaceLayer::onViewportPaused(bool paused)
{
  m_ViewportInfo.paused = paused;
}

void UserInterfaceLayer::fileDropCallback(const Event& e)
{
  VRM_ASSERT(e.type == Event::Type::FileDrop);

  m_fileDrop.cursorPos = Application::Get().getWindow().getCursorPos();
  m_fileDropData = e.stringData;
  m_fileDrop.files = m_fileDropData.c_str();
}
