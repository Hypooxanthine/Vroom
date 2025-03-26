#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "VroomEditor/UserInterface/EntityEditor/EntityEditor.h"

namespace vrm
{

class Entity;
class Scene;

class SceneGraph : public ImGuiElement
{
public:
  SceneGraph();
  virtual ~SceneGraph();

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
  };

  EntityEditor m_entityEditor;
  FrameContext m_frameContext;
};

} // namespace vrm
