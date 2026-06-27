#include "Editor/PointLightComponentEditor.h"

#include <limits>

#include "imgui.h"

using namespace vrm;

void PointLightComponentEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  auto color = component.getColor();
  if (ImGui::ColorEdit3("Color", &color.x))
  {
    component.setColor(color);
  }

  auto intensity = component.getIntensity();
  if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f",
                       ImGuiSliderFlags_AlwaysClamp))
  {
    component.setIntensity(intensity);
  }

  auto radius = component.getRadius();
  if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f",
                       ImGuiSliderFlags_AlwaysClamp))
  {
    component.setRadius(radius);
  }

  auto smoothRadius = component.getSmoothRadius();
  if (ImGui::DragFloat("Smooth radius", &smoothRadius, 0.01f, 0.f, 1.f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
  {
    component.setSmoothRadius(smoothRadius);
  }

  auto constantAttenuation = component.getConstantAttenuation();
  if (ImGui::DragFloat("Constant Attenuation", &constantAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(),
                       "%.3f", ImGuiSliderFlags_AlwaysClamp))
  {
    component.setConstantAttenuation(constantAttenuation);
  }

  auto linearAttenuation = component.getLinearAttenuation();
  if (ImGui::DragFloat("Linear Attenuation", &linearAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(), "%.3f",
                       ImGuiSliderFlags_AlwaysClamp))
  {
    component.setLinearAttenuation(linearAttenuation);
  }

  auto quadraticAttenuation = component.getQuadraticAttenuation();
  if (ImGui::DragFloat("Quadratic Attenuation", &quadraticAttenuation, 0.01f, 0.f, std::numeric_limits<float>::max(),
                       "%.3f", ImGuiSliderFlags_AlwaysClamp))
  {
    component.setQuadraticAttenuation(quadraticAttenuation);
  }
}
