#include "VroomEditor/UserInterface/MainMenuBar.h"
#include <imgui.h>

#include <Vroom/Core/Application.h>

#include "VroomEditor/EditorLayer.h"

#include "Vroom/Core/Profiling.h"

using namespace vrm;

MainMenuBar::MainMenuBar() {}

MainMenuBar::~MainMenuBar() {}

void MainMenuBar::onImgui()
{
  VRM_PROFILE_SCOPE("MainMenuBar::onImgui");

  auto& ui = UserInterfaceLayer::Get();

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New project")) { notImplemented(); }
      if (ImGui::MenuItem("Open project")) { notImplemented(); }
      if (ImGui::MenuItem("Save project")) { notImplemented(); }
      if (ImGui::MenuItem("Save project as")) { notImplemented(); }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) { Application::Get().exit(); }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo")) { notImplemented(); }
      if (ImGui::MenuItem("Redo")) { notImplemented(); }

      ImGui::Separator();

      if (ImGui::MenuItem("Cut")) { notImplemented(); }
      if (ImGui::MenuItem("Copy")) { notImplemented(); }
      if (ImGui::MenuItem("Paste")) { notImplemented(); }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
      EInterfaceElement::Element elem;

      elem = EInterfaceElement::eViewport;
      if (ImGui::MenuItem("Viewport", nullptr, ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eSceneGraph;
      if (ImGui::MenuItem("Scene graph", nullptr, ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eAssetBrowser;
      if (ImGui::MenuItem("Asset browser", nullptr, ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eParticleSystemEditor;
      if (ImGui::MenuItem("Particle system editor", nullptr,
                          ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eEditorPreferences;
      if (ImGui::MenuItem("Editor preferences", nullptr,
                          ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eRenderSettingsPanel;
      if (ImGui::MenuItem("Render settings", nullptr,
                          ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      elem = EInterfaceElement::eProfilingPanel;
      if (ImGui::MenuItem("Profiling", nullptr, ui.isElementVisible(elem)))
      {
        ui.setElementVisible(elem, !ui.isElementVisible(elem));
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Scene"))
    {
      if (ImGui::MenuItem("Unload scene")) { EditorLayer::Get().unloadScene(); }
      if (ImGui::MenuItem("Save scene")) { EditorLayer::Get().saveScene(); }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Scripts"))
    {
      if (ImGui::MenuItem("Build")) { EditorLayer::Get().buildScripts(); }
      if (ImGui::MenuItem("Reload")) { EditorLayer::Get().reloadScripts(); }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About", nullptr))
      {
        /// @todo Redirect to Github page
        notImplemented();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (ImGui::BeginPopup("Not implemented"))
  {
    ImGui::Text("This feature is not implemented yet.");
    ImGui::EndPopup();
  }
}
