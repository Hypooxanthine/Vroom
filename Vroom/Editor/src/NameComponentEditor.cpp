#include "Editor/NameComponentEditor.h"

#include <Scene/Scene.h>

#include "imgui.h"
#include "imgui_internal.h"
#include <misc/cpp/imgui_stdlib.h>

using namespace vrm;

void NameComponentEditor::editEntityComponent(Entity& e) const
{
  constexpr size_t bufferSize = 256;
  std::string      name;
  name.reserve(bufferSize);
  name = e.getName();

  constexpr auto flags =
    ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

  if (!e.getParent().isValid())
    ImGui::BeginDisabled();

  if (ImGui::InputText("##Name", &name, flags))
  {
    if (name.empty() || e.getScene()->entityExists(name))
    {
      if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
      {
        state->ReloadUserBufAndKeepSelection();
      }
    }
    else
    {
      e.setName(name);
    }
  }

  if (!e.getParent().isValid())
  {
    ImGui::EndDisabled();
    ImGui::SetItemTooltip("You cannot edit Root entity name.");
  }
}
