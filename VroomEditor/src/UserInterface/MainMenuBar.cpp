#include "VroomEditor/UserInterface/MainMenuBar.h"

#include <imgui.h>

#include <Vroom/Core/Application.h>

#include "VroomEditor/EditorLayer.h"

using namespace vrm;

MainMenuBar::MainMenuBar()
{
}

MainMenuBar::~MainMenuBar()
{
}

bool MainMenuBar::onImgui()
{
  bool ret = false;

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New project", "Ctrl+N"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Open project", "Ctrl+O"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Save project", "Ctrl+S"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Save project as", "Ctrl+Shift+S"))
      {
        notImplemented();
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit", "Alt+F4"))
      {
        Application::Get().exit();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo", "Ctrl+Z"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z"))
      {
        notImplemented();
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Cut", "Ctrl+X"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Copy", "Ctrl+C"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Paste", "Ctrl+V"))
      {
        notImplemented();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem("Scene", "Ctrl+1"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Properties", "Ctrl+2"))
      {
        notImplemented();
      }
      if (ImGui::MenuItem("Console", "Ctrl+3"))
      {
        notImplemented();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Scene"))
    {
      if (ImGui::MenuItem("Unload scene"))
      {
        EditorLayer::Get().unloadScene();
      }
      if (ImGui::MenuItem("Save scene"))
      {
        EditorLayer::Get().saveScene();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About", "Ctrl+F1"))
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

  return ret;
}
