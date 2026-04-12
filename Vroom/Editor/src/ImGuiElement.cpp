#include "Editor/ImGuiElement.h"

#include <imgui.h>
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Core/Log.h>

namespace vrm
{

ImGuiElement::ImGuiElement()
{
}

ImGuiElement::~ImGuiElement()
{
}

void ImGuiElement::renderImgui()
{
  if (m_open == nullptr || *m_open == true)
    onImgui();
}

void ImGuiElement::notImplemented()
{
  VRM_LOG_WARN("Not implemented");
}

} // namespace vrm