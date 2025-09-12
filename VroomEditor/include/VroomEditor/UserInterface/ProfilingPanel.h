#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm::editor
{

class ProfilingPanel : public ImGuiElement
{
public:

  ProfilingPanel();
  ~ProfilingPanel();

  ProfilingPanel& operator=(const ProfilingPanel& other) = delete;
  ProfilingPanel(const ProfilingPanel& other)            = delete;

  ProfilingPanel& operator=(ProfilingPanel&& other) = delete;
  ProfilingPanel(ProfilingPanel&& other)            = delete;

protected:

  void onImgui() override;

public:
};

} // namespace vrm::editor
