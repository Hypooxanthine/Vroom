#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class Entity;

class SceneGraph : public ImGuiElement
{
public:
  SceneGraph();
  virtual ~SceneGraph();

protected:

  virtual bool onImgui() override;

  void renderEntityEntryRecursive(const Entity& e);

private:

};

} // namespace vrm
