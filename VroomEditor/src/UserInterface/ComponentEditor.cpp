#include "VroomEditor/UserInterface/ComponentEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string_view>

#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/AssetManager.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>

#include "VroomEditor/EditorLayer.h"
#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

//--------------------------------------------------
// Include necessary component headers

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
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
 * @param removable (Optional) in {true, false}, to tell if the component can be removed or not
 * 
 */
#define VRM_REGISTER_COMPONENT_EDITOR(ComponentName, displayName, removable) \
  class ComponentName##Editor : public ComponentEditor, public ComponentGetter<ComponentName>\
  {\
  public:\
    ~ComponentName##Editor() {}\
    bool editEntityComponent(Entity& ) const override;\
    std::string getComponentName() const override { return displayName; }\
    bool canEditEntity(const Entity& e) const override { return has(e); }\
    bool canBeRemoved() const override { return removable; }\
    void removeFromEntity(Entity& e) const override { if constexpr (removable) e.removeComponent<ComponentName>(); }\
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


//--------------------------------------------------
// Implementations of component editors

VRM_REGISTER_COMPONENT_EDITOR(NameComponent, "Name tag", false)
bool NameComponentEditor::editEntityComponent(Entity& e) const
{
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

VRM_REGISTER_COMPONENT_EDITOR(TransformComponent, "Transform component", false)
bool TransformComponentEditor::editEntityComponent(Entity& e) const
{
  auto &component = get(e);

  auto pos = component.getPosition();
  auto rot = component.getRotation() * 180.0f / glm::pi<float>();
  auto scale = component.getScale();
  
  if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
    component.setPosition(pos);
  if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
    component.setRotation(rot * glm::pi<float>() / 180.0f);
  if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
    component.setScale(scale);

  const auto& camera = EditorLayer::Get().getCurrentCamera();
  glm::mat4 transform = component.getGlobalTransform();

  auto& ui = UserInterfaceLayer::Get();
  auto& viewportInfo = ui.getViewportInfo();
  
  if (ImGuizmo::Manipulate(
    &camera.getView()[0][0],
    &camera.getProjection()[0][0],
    ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE,
    viewportInfo.localSpace ? ImGuizmo::MODE::LOCAL : ImGuizmo::MODE::WORLD,
    &transform[0][0]
  ))
  {
    if (e.isRoot())
      component.setTransform(transform);
    else
      component.setGlobalTransform(transform, get(e.getParent()).getGlobalTransform());
      
    viewportInfo.manipulatingGuizmo = true;
    // Cancelling viewport active
    viewportInfo.active = false;
  }
  else if (ImGuizmo::IsUsing())
  {
    viewportInfo.manipulatingGuizmo = true;
    viewportInfo.active = false;
  }
  else if (!viewportInfo.active) // If viewport is not active any more (mouse released), stop manipulating
  {
    viewportInfo.manipulatingGuizmo = false;
  }

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(DirectionalLightComponent, "Directional light component", true)
bool DirectionalLightComponentEditor::editEntityComponent(Entity& e) const
{
  auto &component = get(e);

  auto color = component.color;
  auto intensity = component.intensity;

  if (ImGui::ColorEdit3("Color", &color.x))
    component.color = color;
  if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
    component.intensity = intensity;
  ImGui::Checkbox("Casts shadows", &component.castsShadows);

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(PointLightComponent, "Point light component", true)
bool PointLightComponentEditor::editEntityComponent(Entity& e) const
{
  auto &component = get(e);
  
  auto color = component.color;
  auto intensity = component.intensity;
  auto radius = component.radius;
  auto constantAttenuation = component.constantAttenuation;
  auto linearAttenuation = component.linearAttenuation;
  auto quadraticAttenuation = component.quadraticAttenuation;

  if (ImGui::ColorEdit3("Color", &color.x))
    component.color = color;
  if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f", ImGuiSliderFlags_AlwaysClamp))
    component.intensity = intensity;
  if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f", ImGuiSliderFlags_AlwaysClamp))
    component.radius = radius;
  if (ImGui::DragFloat("Constant Attenuation", &constantAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp))
    component.constantAttenuation = constantAttenuation;
  if (ImGui::DragFloat("Linear Attenuation", &linearAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp))
    component.linearAttenuation = linearAttenuation;
  if (ImGui::DragFloat("Quadratic Attenuation", &quadraticAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp))
    component.quadraticAttenuation = quadraticAttenuation;

  return true;
}

VRM_REGISTER_COMPONENT_EDITOR(MeshComponent, "Mesh component", true)
bool MeshComponentEditor::editEntityComponent(Entity& e) const
{
  auto &component = get(e);

  constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

  MeshAsset::Handle requestNewMesh;
  std::pair<size_t, MaterialAsset::Handle> requestNewMaterial;

  std::string resourceName = component.getMesh()->getFilePath();
  bool meshChanged = false;
  if (ImGui::InputText("Mesh", &resourceName, flags))
  {
    if (AssetManager::Get().tryLoadAsset<MeshAsset>(resourceName))
    {
      meshChanged = true;
    }
    else
    {
      if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
      {
        state->ReloadUserBufAndKeepSelection();
      }
    }
  }

  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshAsset"))
    {
      // Entity* newChild = (Entity*)payload->Data;
      std::filesystem::path meshPath = *(std::filesystem::path*)payload->Data;

      if (AssetManager::Get().tryLoadAsset<MeshAsset>(meshPath.string()))
      {
        resourceName = meshPath.string();
        meshChanged = true;
      }
    }

    ImGui::EndDragDropTarget();
  }

  if (meshChanged)
  {
    requestNewMesh = AssetManager::Get().getAsset<MeshAsset>(resourceName);
  }

  const size_t matSlotMax = component.getMaterials().getSlotCount();
  for (size_t matSlot = 0; matSlot < matSlotMax; ++matSlot)
  {
    // return false;
    const auto& mat = component.getMaterials().getMaterial(matSlot);
    if (!mat.isValid()) continue;
    std::string inputName = "Material " + std::to_string(matSlot);
    std::string resourceName = mat->getFilePath();
    bool resourceChanged = false;
    
    constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

    if (ImGui::InputText(inputName.c_str(), &resourceName, flags))
    {
      if (AssetManager::Get().tryLoadAsset<MaterialAsset>(resourceName))
      {
        resourceChanged = true;
      }
      else
      {
        if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
        {
          state->ReloadUserBufAndKeepSelection();
        }
      }
    }

    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MaterialAsset"))
      {
        // Entity* newChild = (Entity*)payload->Data;
        std::filesystem::path materialPath = *(std::filesystem::path*)payload->Data;

        if (AssetManager::Get().tryLoadAsset<MaterialAsset>(materialPath.string()))
        {
          resourceName = materialPath.string();
          resourceChanged = true;
        }
      }

      ImGui::EndDragDropTarget();
    }

    if (resourceChanged)
    {
      requestNewMaterial.first = matSlot;
      requestNewMaterial.second = AssetManager::Get().getAsset<MaterialAsset>(resourceName);
    }
  }

  bool visible = component.isVisible();
  if (ImGui::Checkbox("Visible", &visible))
    component.setVisible(visible);

  bool castsShadow = component.doesCastShadow();
  if (ImGui::Checkbox("Casts shadow", &castsShadow))
    component.setCastsShadow(castsShadow);

  if (requestNewMesh.isValid())
      component.setMesh(requestNewMesh);
    
  if (requestNewMaterial.second.isValid())
    component.setMaterial(requestNewMaterial.first, requestNewMaterial.second);

  return true;
}
