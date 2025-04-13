#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Core/GameLayer.h>

#include "VroomEditor/EditorLayer.h"

#include "VroomEditor/UserInterface/MainMenuBar.h"
#include "VroomEditor/UserInterface/StatisticsPanel.h"
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

  m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
  VRM_ASSERT_MSG(m_Font, "Failed to load font.");

  // Interfaces setup
  auto& mainMenuBar = emplaceImGuiElement<MainMenuBar>();
  auto& statisticsPanel = emplaceImGuiElement<StatisticsPanel>();
  auto& viewport = emplaceImGuiElement<Viewport>();
  auto& assetBrowser = emplaceImGuiElement<AssetBrowser>();
  auto& sceneGraph = emplaceImGuiElement<SceneGraph>();

  viewport.setRenderTexture(&app.getGameLayer().getFrameBuffer().getColorAttachmentTexture(0));
}

void UserInterfaceLayer::onEnd()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UserInterfaceLayer::onUpdate(float dt)
{

}

void UserInterfaceLayer::onRender()
{
  auto& fb = gl::FrameBuffer::GetDefaultFrameBuffer();
  fb.bind();
  resetUIInfos();
  renderImgui();
}

void UserInterfaceLayer::onEvent(vrm::Event &e)
{

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
