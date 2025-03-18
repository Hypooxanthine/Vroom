#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class SceneGraph : public ImGuiElement
{
public:
  SceneGraph();
  virtual ~SceneGraph();

protected:

  virtual bool onImgui() override;

private:

};

} // namespace vrm
