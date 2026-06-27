#include "Editor/TransformComponentEditor.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "Editor/EditorLayer.h"
#include "Editor/UserInterfaceLayer.h"

using namespace vrm;

void TransformComponentEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  auto pos   = component.getPosition();
  auto rot   = component.getRotation() * 180.0f / glm::pi<float>();
  auto scale = component.getScale();

  if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
    component.setPosition(pos);
  if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
    component.setRotation(rot * glm::pi<float>() / 180.0f);
  if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
    component.setScale(scale);

  const auto& camera    = EditorLayer::Get().getCurrentCamera();
  glm::mat4   transform = component.getGlobalTransform();

  auto& ui           = UserInterfaceLayer::Get();
  auto& viewportInfo = ui.getViewportInfo();

  if (viewportInfo.viewportVisible &&
      ImGuizmo::Manipulate(&camera.getView()[0][0], &camera.getProjection()[0][0],
                           ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE,
                           viewportInfo.localSpace ? ImGuizmo::MODE::LOCAL : ImGuizmo::MODE::WORLD, &transform[0][0]))
  {
    if (e.isRoot())
      component.setTransform(transform);
    else
      component.setGlobalTransform(transform, get(e.getParent()).getGlobalTransform());

    viewportInfo.manipulatingGuizmo = true;
    // Cancelling viewport active
    viewportInfo.active = false;
  }
  else if (viewportInfo.viewportVisible && ImGuizmo::IsUsing())
  {
    viewportInfo.manipulatingGuizmo = true;
    viewportInfo.active             = false;
  }
  else if (!viewportInfo.active) // If viewport is not active any more (mouse
                                 // released), stop manipulating
  {
    viewportInfo.manipulatingGuizmo = false;
  }
}
