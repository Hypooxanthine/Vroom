#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "VroomEditor/UserInterface/EntityEditor/EntityEditor.h"

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

  void renderEntityEntryRecursive(Entity e);

private:

  EntityEditor m_entityEditor;
};

} // namespace vrm
