#include "Editor/DirectionalLightComponentEditor.h"

#include "imgui.h"

using namespace vrm;

void DirectionalLightComponentEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  auto color = component.color;
  auto intensity = component.intensity;

  if (ImGui::ColorEdit3("Color", &color.x))
    component.setColor(color);
  if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
    component.setIntensity(intensity);
  bool castsShadows = component.castsShadows;
  if (ImGui::Checkbox("Casts shadows", &castsShadows))
    component.setCastsShadows(castsShadows);
}
