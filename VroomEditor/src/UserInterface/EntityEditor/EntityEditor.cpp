#include "VroomEditor/UserInterface/EntityEditor/EntityEditor.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Components/NameComponent.h>

#include <imgui.h>

using namespace vrm;

EntityEditor::EntityEditor()
  : ImGuiElement()
{

}

EntityEditor::~EntityEditor()
{

}

void EntityEditor::open(Entity e)
{
  m_open = true;
  m_entity = e;
}

void EntityEditor::openOrCloseIfSame(Entity e)
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

  auto flags = ImGuiWindowFlags_NoFocusOnAppearing;

  if (ImGui::Begin("Entity editor", &m_open, flags))
  {
    ImGui::Text("Edit: %s", m_entity.getComponent<NameComponent>().name.c_str());
  }
  ImGui::End();

  // User requested close
  if (!m_open)
    close();

  return ret;
}
