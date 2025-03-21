#include "VroomEditor/UserInterface/EntityEditor/ComponentEditor.h"

#include <imgui.h>

//--------------------------------------------------
// Include necessary component headers

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"

//--------------------------------------------------
// Preprocessor/template machinery

using namespace vrm;

static std::vector<std::unique_ptr<ComponentEditor>> ALL_EDITORS;

void ComponentEditor::EditEntity(Entity e)
{
  for (auto &editor : ALL_EDITORS)
  {
    editor->editEntityComponent(e);
  }
}

template <typename T>
struct ComponentGetter
{
  using ComponentType = T;

  inline bool has(Entity e) const
  {
    return e.hasComponent<T>();
  }

  inline T& get(Entity e) const
  {
    return e.getComponent<T>();
  }
};

template <typename T>
struct ComponentEditorRegisterer
{
private:
  struct Registerer
  {
    Registerer()
    {
      ALL_EDITORS.emplace_back(std::make_unique<T>());
    }
  };
public:

  inline static Registerer reg;
};

/**
 * @brief Macro to register and implement a component editor
 * @param ComponentName The name of the component
 * @param entityName The name of the entity variable in the editor function
 * 
 */
#define VRM_REGISTER_COMPONENT_EDITOR(ComponentName, entityName) \
  class ComponentName##Editor : public ComponentEditor, public ComponentGetter<ComponentName>, public ComponentEditorRegisterer<ComponentName##Editor>\
  {\
  public:\
    void editEntityComponent(Entity entityName) const override;\
  };\
  void ComponentName##Editor::editEntityComponent(Entity entityName) const\


//--------------------------------------------------
// Implementations of component editors

VRM_REGISTER_COMPONENT_EDITOR(TransformComponent, e)
{
  if (!has(e))
    return;
  auto &component = get(e);

  ImGui::Text("Transform");

  auto pos = component.getPosition();
  auto rot = component.getRotation();
  auto scale = component.getScale();
  
  if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
    component.setPosition(pos);
  if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
    component.setRotation(rot);
  if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
    component.setScale(scale);
}

VRM_REGISTER_COMPONENT_EDITOR(PointLightComponent, e)
{
  if (!has(e))
    return;
  auto &component = get(e);

  ImGui::Text("Point light");
  
  auto color = component.color;
  auto intensity = component.intensity;
  auto radius = component.radius;

  if (ImGui::ColorEdit3("Color", &color.x))
    component.color = color;
  if (ImGui::DragFloat("Intensity", &intensity, 0.1f))
    component.intensity = intensity;
  if (ImGui::DragFloat("Radius", &radius, 0.1f))
    component.radius = radius;
}

VRM_REGISTER_COMPONENT_EDITOR(NameComponent, e)
{
  if (!has(e))
    return;
  auto &component = get(e);

  ImGui::Text("Name");

  constexpr size_t bufferSize = 256;
  std::string buffer = component.name;
  buffer.resize(bufferSize);

  if (ImGui::InputText("Name", buffer.data(), bufferSize))
    component.name = buffer;
}
