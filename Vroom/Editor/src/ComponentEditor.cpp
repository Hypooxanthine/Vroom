#include "Editor/ComponentEditor.h"

#include <memory>
#include <string_view>
#include <vector>

#include "imgui.h"

#include "Core/Profiling.h"

//--------------------------------------------------
// Concrete component editors

#include "Editor/DirectionalLightComponentEditor.h"
#include "Editor/MeshComponentEditor.h"
#include "Editor/NameComponentEditor.h"
#include "Editor/PointLightComponentEditor.h"
#include "Editor/ScriptHandlerEditor.h"
#include "Editor/SkyboxComponentEditor.h"
#include "Editor/TransformComponentEditor.h"

using namespace vrm;

namespace
{

template <typename C, typename... Args>
void AddComponentItem(Entity& e, std::string_view displayName, Args&&... args)
{
  if (!e.hasComponent<C>() && ImGui::Selectable(displayName.data()))
  {
    e.addComponent<C>(std::forward<Args>(args)...);
  }
}

// The single place listing every component editor. Add a new editor here (and
// include its header above) to register it.
std::vector<std::unique_ptr<ComponentEditor>> makeAllEditors()
{
  std::vector<std::unique_ptr<ComponentEditor>> editors;
  editors.push_back(std::make_unique<NameComponentEditor>());
  editors.push_back(std::make_unique<TransformComponentEditor>());
  editors.push_back(std::make_unique<DirectionalLightComponentEditor>());
  editors.push_back(std::make_unique<PointLightComponentEditor>());
  editors.push_back(std::make_unique<MeshComponentEditor>());
  editors.push_back(std::make_unique<SkyboxComponentEditor>());
  editors.push_back(std::make_unique<ScriptHandlerEditor>());
  return editors;
}

} // namespace

void ComponentEditor::EditEntity(Entity& e)
{
  VRM_PROFILE_SCOPE("ComponentEditor::EditEntity");

  static const std::vector<std::unique_ptr<ComponentEditor>> editors = makeAllEditors();

  for (const auto& editor : editors)
  {
    if (!editor->canEditEntity(e))
      continue;

    ImGui::SeparatorText(editor->getComponentName().c_str());

    bool askedRemove = false;
    ImGui::PushID(editor->getComponentName().c_str());
    if (editor->canBeRemoved() && ImGui::BeginPopupContextItem("Popup"))
    {
      if (ImGui::Selectable("Remove component"))
      {
        askedRemove = true;
      }

      ImGui::EndPopup();
    }

    editor->editEntityComponent(e);

    ImGui::PopID();

    if (askedRemove)
      editor->removeFromEntity(e);
  }

  ImGui::Separator();

  if (!e.isRoot() && ImGui::BeginCombo("##Add component", "Add component"))
  {
    AddComponentItem<DirectionalLightComponent>(e, "Directional light");
    AddComponentItem<PointLightComponent>(e, "Point light");
    AddComponentItem<MeshComponent>(e, "Mesh");
    AddComponentItem<SkyboxComponent>(e, "Skybox");

    AddComponentItem<ScriptHandler>(e, "Script");

    ImGui::EndCombo();
  }
}
