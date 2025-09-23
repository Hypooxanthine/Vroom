#include "VroomEditor/UserInterface/UserInterfaceLayer.h"
#include <fstream>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/Assert.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Render/Renderer.h>

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "ImGuizmo.h"

#include "VroomEditor/EditorLayer.h"
#include "VroomEditor/Serial/Json.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"
#include "VroomEditor/UserInterface/EditorPreferences.h"
#include "VroomEditor/UserInterface/EntityEditor.h"
#include "VroomEditor/UserInterface/MainMenuBar.h"
#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "VroomEditor/UserInterface/ProfilingPanel.h"
#include "VroomEditor/UserInterface/RenderSettingsPanel.h"
#include "VroomEditor/UserInterface/SceneGraph.h"
#include "VroomEditor/UserInterface/Viewport.h"

#include "Vroom/Core/Profiling.h"
#include "implot.h"

using namespace vrm;

static UserInterfaceLayer* INSTANCE;

UserInterfaceLayer::UserInterfaceLayer() : Layer(), m_Font(nullptr)
{
  VRM_ASSERT_MSG(INSTANCE == nullptr,
                 "Only one instance of UserInterfaceLayer is allowed");
  INSTANCE = this;

  m_openFlags.fill(true);
}

UserInterfaceLayer::~UserInterfaceLayer() {}

UserInterfaceLayer& UserInterfaceLayer::Get() { return *INSTANCE; }

void UserInterfaceLayer::onInit()
{
  auto& app = Application::Get();

  VRM_LOG_INFO("UserInterfaceLayer init");

  // Imgui setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io     = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplGlfw_InitForOpenGL(Application::Get().getWindow().getGLFWHandle(),
                               true);
  ImGui_ImplOpenGL3_Init("#version 450");

  m_Font = io.Fonts->AddFontFromFileTTF(
    "Resources/Editor/Appearance/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
  VRM_ASSERT_MSG(m_Font, "Failed to load font.");

  // Interfaces setup
  emplaceImGuiElement<MainMenuBar>(EInterfaceElement::eMainMenuBar);
  emplaceImGuiElement<editor::ProfilingPanel>(
    EInterfaceElement::eProfilingPanel);
  emplaceImGuiElement<RenderSettingsPanel>(
    EInterfaceElement::eRenderSettingsPanel);
  emplaceImGuiElement<Viewport>(EInterfaceElement::eViewport);
  emplaceImGuiElement<AssetBrowser>(EInterfaceElement::eAssetBrowser);
  emplaceImGuiElement<SceneGraph>(EInterfaceElement::eSceneGraph);
  emplaceImGuiElement<EditorPreferences>(EInterfaceElement::eEditorPreferences,
                                         false);
  emplaceImGuiElement<EntityEditor>(EInterfaceElement::eEntityEditor);
  emplaceImGuiElement<editor::ParticleSystemEditor>(
    EInterfaceElement::eParticleSystemEditor, false);

  m_CustomEventManager.createCustomEvent("OSFileDrop")
    .bindInput(Event::Type::FileDrop);
  m_CustomEventManager.bindPermanentCallback("OSFileDrop",
                                             [this](const Event& e)
                                             {
                                               this->fileDropCallback(e);
                                               e.handled = true;
                                             });

  m_CustomEventManager.createCustomEvent("OSDragEnter")
    .bindInput(Event::Type::FileDragEnter);
  m_CustomEventManager.bindPermanentCallback("OSDragEnter",
                                             [this](const Event& e)
                                             {
                                               this->m_isDraggingOSFile = true;
                                               e.handled                = true;
                                             });

  m_CustomEventManager.createCustomEvent("OSDragLeave")
    .bindInput(Event::Type::FileDragLeave);
  m_CustomEventManager.bindPermanentCallback("OSDragLeave",
                                             [this](const Event& e)
                                             {
                                               this->m_isDraggingOSFile = false;
                                               e.handled                = true;
                                             });

  ImGui::StyleColorsDark();
  if (!_loadImguiStyle()) { ImGui::StyleColorsDark(); }
}

void UserInterfaceLayer::onEnd()
{
  saveImguiStyle(ImGui::GetStyle());

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImPlot::DestroyContext();
  ImGui::DestroyContext();
}

void UserInterfaceLayer::onUpdate(const DeltaTime& dt)
{
  m_fileDrop.clear();
  auto& viewport = VRM_EDITOR_UI_ELEMENT(Viewport);
  viewport.setRenderTexture(Application::Get()
                              .getMainSceneRenderer()
                              .getRenderPipeline()
                              .getRenderedTexture());

  for (auto& element : m_elements)
  {
    if (element) element->update(dt);
  }
}

void UserInterfaceLayer::onRender()
{
  VRM_PROFILE_SCOPE("UserInterfaceLayer::onRender");
  for (auto& element : m_elements)
  {
    if (element) element->render();
  }

  gl::FrameBuffer::GetDefaultFrameBuffer().bind();
  renderImgui();
}

void UserInterfaceLayer::onEvent(vrm::Event& e)
{
  m_CustomEventManager.check(e);
}

void UserInterfaceLayer::renderImgui()
{
  VRM_PROFILE_SCOPE("UserInterfaceLayer::renderImgui");

  _beginImguiFrame();

  ImGui::PushFont(m_Font);

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  if (m_isDraggingOSFile || m_fileDrop.files)
  {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
    {
      if (m_fileDrop.files)
        ImGui::SetDragDropPayload("OSFileDrop", &m_fileDrop,
                                  sizeof(m_fileDrop));
      else ImGui::SetDragDropPayload("OSFileDrop", nullptr, 0);
      ImGui::EndDragDropSource();
    }
  }

  for (auto& element : m_elements)
  {
    if (element) element->renderImgui();
  }

  ImGui::PopFont();
  _endImguiFrame();
}

void UserInterfaceLayer::_beginImguiFrame()
{
  VRM_PROFILE_SCOPE("UserInterfaceLayer::_beginImguiFrame");

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
}

void UserInterfaceLayer::_endImguiFrame()
{
  VRM_PROFILE_SCOPE("UserInterfaceLayer::_endImguiFrame");

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterfaceLayer::fileDropCallback(const Event& e)
{
  VRM_ASSERT(e.type == Event::Type::FileDrop);

  m_fileDrop.cursorPos = Application::Get().getWindow().getCursorPos();
  m_fileDropData       = e.stringData;
  m_fileDrop.files     = m_fileDropData.c_str();
}

bool UserInterfaceLayer::_loadImguiStyle()
{
  std::ifstream ifs;
  ifs.open(s_imguiStyleFile);

  if (ifs.is_open())
  {
    json j;

    try
    {
      ifs >> j;
    } catch (const std::exception& e)
    {
      VRM_LOG_ERROR("Could not parse imgui style {} into json. Error:\n{}",
                    s_imguiStyleFile.string(), e.what());
      return false;
    }

    try
    {
      ImGuiStyle& style = ImGui::GetStyle();
      nlohmann::adl_serializer<ImGuiStyle>().from_json(j, style);
    } catch (const std::exception& e)
    {
      VRM_LOG_ERROR(
        "Could not parse imgui style {} into an ImGuiStyle. Error:\n{}",
        s_imguiStyleFile.string(), e.what());
      return false;
    }
  }
  else
  {
    VRM_LOG_WARN("Imgui style {} not found", s_imguiStyleFile.string());
    return false;
  }

  return true;
}

void UserInterfaceLayer::saveImguiStyle(const ImGuiStyle& style) const
{
  try
  {
    if (!std::filesystem::exists(s_imguiStyleFile.parent_path()))
      std::filesystem::create_directories(s_imguiStyleFile.parent_path());
  } catch (const std::filesystem::filesystem_error& e)
  {
    VRM_LOG_WARN("Could not create directory {}. Error: {}",
                 s_imguiStyleFile.parent_path().string(), e.what());
    return;
  }

  std::ofstream ofs;
  ofs.open(s_imguiStyleFile, std::ofstream::trunc);

  if (ofs.is_open())
  {
    json j;

    nlohmann::adl_serializer<ImGuiStyle>().to_json(j, style);

    ofs << j;
  }
  else
  {
    VRM_LOG_WARN("Could not open or create {} file", s_imguiStyleFile.string());
  }
}

Viewport& UserInterfaceLayer::getViewport()
{
  return VRM_EDITOR_UI_ELEMENT(Viewport);
}
