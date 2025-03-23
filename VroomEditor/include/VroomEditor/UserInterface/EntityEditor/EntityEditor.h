#pragma once

#include <Vroom/Scene/Entity.h>

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class Entity;

class EntityEditor : public ImGuiElement
{
public:
  EntityEditor();
  virtual ~EntityEditor();

  void open(Entity& e);
  void openOrCloseIfSame(Entity& e);
  void close();

  bool isEditingEntity(const Entity& e) const;

protected:

  virtual bool onImgui() override;

private:
  bool m_open = false;
  Entity m_entity;
};

} // namespace vrm
