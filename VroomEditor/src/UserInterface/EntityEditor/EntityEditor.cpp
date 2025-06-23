#include "VroomEditor/UserInterface/EntityEditor/EntityEditor.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Components/NameComponent.h>

#include <imgui.h>

#include "VroomEditor/UserInterface/EntityEditor/ComponentEditor.h"

using namespace vrm;

EntityEditor::EntityEditor()
  : ImGuiElement()
{

}

EntityEditor::~EntityEditor()
{

}

void EntityEditor::open(Entity& e)
{
  m_open = true;
  m_entity = e.clone();
}

void EntityEditor::openOrCloseIfSame(Entity& e)
{
  if (m_entity == e)
    close();
  else
    open(e);
}

void EntityEditor::close()
{
  m_open = false;
  m_entity = Entity();
}

bool EntityEditor::isEditingEntity(const Entity& e) const
{
  return m_entity == e;
}

bool EntityEditor::onImgui()
{
  if (!m_entity.isValid())
    close();

  if (!m_open)
    return false;

  bool ret = false;

  constexpr auto flags = ImGuiWindowFlags_None
    ;
  
  if (ImGui::Begin("Entity editor", &m_open, flags))
  {
    ComponentEditor::EditEntity(m_entity);
  }
  ImGui::End();

  // User requested close
  if (!m_open)
    close();

  return ret;
}
