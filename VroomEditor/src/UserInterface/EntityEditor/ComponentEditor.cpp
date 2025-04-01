#include "VroomEditor/UserInterface/EntityEditor/ComponentEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/AssetManager.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>

//--------------------------------------------------
// Include necessary component headers

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"

//--------------------------------------------------
// Preprocessor/template machinery

using namespace vrm;

static std::vector<std::unique_ptr<ComponentEditor>> ALL_EDITORS = {};

template <typename C, typename... Args>
static inline void AddComponentItem(Entity& e, const std::string_view& displayName, Args&&... args)
{
  if (!e.hasComponent<C>() && ImGui::Selectable(displayName.data()))
  {
    e.addComponent<C>(std::forward<Args>(args)...);
  }
}

void ComponentEditor::EditEntity(Entity& e)
{
  for (auto &editor : ALL_EDITORS)
  {
    editor->editEntityComponent(e);
  }

  ImGui::Separator();

  if (!e.isRoot() && ImGui::BeginCombo("##Add component", "Add component"))
  {
    AddComponentItem<PointLightComponent>(e, "Point light");
    AddComponentItem<MeshComponent>(e, "Mesh");

    ImGui::EndCombo();
  }
}

template <typename T>
struct ComponentGetter
{
  using ComponentType = T;

  inline bool has(const Entity& e) const
  {
    return e.hasComponent<T>();
  }

  inline T& get(Entity& e) const
  {
    return e.getComponent<T>();
  }
};

/**
 * @brief Macro to register and implement a component editor
 * @param ComponentName The name of the component
 * @param entityName The name of the entity variable in the editor function
 * 
 */
#define VRM_REGISTER_COMPONENT_EDITOR(ComponentName, entityName) \
  class ComponentName##Editor : public ComponentEditor, public ComponentGetter<ComponentName>\
  {\
  public:\
    ~ComponentName##Editor() {}\
    bool editEntityComponent(Entity& entityName) const override;\
  private:\
    ComponentName##Editor ()\
      : ComponentEditor(), ComponentGetter<ComponentName>()\
    {}\
    friend struct Registerer;\
    struct Registerer\
    {\
      Registerer()\
      {\
        ALL_EDITORS.emplace_back().reset(new ComponentName##Editor ());\
      }\
    };\
\
    static Registerer s_Registerer;\
  };\
  ComponentName##Editor ::Registerer ComponentName##Editor ::s_Registerer = {};\
  bool ComponentName##Editor::editEntityComponent(Entity& entityName) const


//--------------------------------------------------
// Implementations of component editors

VRM_REGISTER_COMPONENT_EDITOR(NameComponent, e)
{
  ImGui::SeparatorText("Name tag");

  constexpr size_t bufferSize = 256;
  std::string name;
  name.reserve(bufferSize);
  name = e.getName();

  constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

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

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(TransformComponent, e)
{
  if (!has(e))
    return false;
  auto &component = get(e);

  ImGui::SeparatorText("Transform component");

  auto pos = component.getPosition();
  auto rot = component.getRotation() * 180.0f / glm::pi<float>();
  auto scale = component.getScale();
  
  if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
    component.setPosition(pos);
  if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
    component.setRotation(rot * glm::pi<float>() / 180.0f);
  if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
    component.setScale(scale);

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(PointLightComponent, e)
{
  if (!has(e))
    return false;
  auto &component = get(e);

  ImGui::SeparatorText("Point light component");
  
  auto color = component.color;
  auto intensity = component.intensity;
  auto radius = component.radius;

  if (ImGui::ColorEdit3("Color", &color.x))
    component.color = color;
  if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f", ImGuiSliderFlags_AlwaysClamp))
    component.intensity = intensity;
  if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f", ImGuiSliderFlags_AlwaysClamp))
    component.radius = radius;

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(MeshComponent, e)
{
  if (!has(e))
    return false;
  auto &component = get(e);

  ImGui::SeparatorText("Mesh component");

  bool askedRemove = false;
  if (ImGui::BeginPopupContextItem("Popup"))
  {
    if (ImGui::Selectable("Remove component"))
    {
      askedRemove = true;
    }

    ImGui::EndPopup();
  }

  constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

  std::string resourceName = component.getMesh()->getFilePath();
  if (ImGui::InputText("Mesh", &resourceName, flags))
  {
    if (AssetManager::Get().tryLoadAsset<MeshAsset>(resourceName))
    {
      component.setMesh(AssetManager::Get().getAsset<MeshAsset>(resourceName));
    }
    else
    {
      if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
      {
        state->ReloadUserBufAndKeepSelection();
      }
    }
  }

  bool visible = component.isVisible();
  if (ImGui::Checkbox("Visible", &visible))
    component.setVisible(visible);

  if (askedRemove)
    e.removeComponent<MeshComponent>();

  return true;
}
