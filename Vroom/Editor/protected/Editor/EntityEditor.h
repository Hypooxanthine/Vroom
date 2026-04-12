#pragma once

#include <Scene/Entity.h>

#include "Editor/ImGuiElement.h"

namespace vrm
{

  class Entity;

  class EntityEditor : public ImGuiElement
  {
  public:

    static void ContextualMenuBehaviour(const Entity& e);
    static void RequestDeleteEntity(const Entity& e);
    static void RequestCreateEntityChild(const Entity& parent);

  public:
    EntityEditor();
    virtual ~EntityEditor();

    void open(const Entity& e);
    void openOrCloseIfSame(const Entity& e);
    void close();

    bool isEditingEntity(const Entity& e) const;

  protected:

    virtual void onImgui() override;

  private:
    bool m_open = false;
    Entity m_entity;
  };

} // namespace vrm
