#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <imgui.h>
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Vroom/Core/Log.h>

namespace vrm
{

ImGuiElement::ImGuiElement()
{
}

ImGuiElement::~ImGuiElement()
{
}

bool ImGuiElement::renderImgui()
{
  if (m_open == nullptr || *m_open == true)
    return onImgui();
  else
    return false;
}

void ImGuiElement::notImplemented()
{
    VRM_LOG_WARN("Not implemented");
}

} // namespace vrm