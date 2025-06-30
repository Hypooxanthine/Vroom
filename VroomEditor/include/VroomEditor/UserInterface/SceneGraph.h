#pragma once

#include <Vroom/Scene/Entity.h>

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class Entity;
class Scene;
class EntityEditor;

class SceneGraph : public ImGuiElement
{
public:
  SceneGraph();
  virtual ~SceneGraph();

  void selectEntity(Entity& e);
  void unselectEntity();

protected:

  virtual bool onImgui() override;

  void renderEntityEntryRecursive(Entity& e);
  void clickBehaviour(Entity& e);
  void contextualMenuBehaviour(Entity& e);
  void dragAndDropBehaviour(Entity& e);

private:

  void setupFrameContext();
  void handleFrameContext();

private:

  struct FrameContext
  {
    struct
    {
      Entity parent, child;
    } RequestHierarchyEdit;

    Entity deletedEntity;
    Entity requestNewChild;

    Scene* activeScene = nullptr;
    EntityEditor* entityEditor;
  };

  FrameContext m_frameContext;
};

} // namespace vrm
