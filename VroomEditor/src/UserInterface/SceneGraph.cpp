#include "VroomEditor/UserInterface/SceneGraph.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/HierarchyComponent.h>
#include <Vroom/Scene/Components/NameComponent.h>

#include "VroomEditor/UserInterface/EntityEditor.h"
#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

#include <imgui.h>

using namespace vrm;

SceneGraph::SceneGraph()
{

}

SceneGraph::~SceneGraph()
{

}

bool SceneGraph::onImgui()
{
  bool ret = false;

  setupFrameContext();

  auto& scene = *m_frameContext.activeScene;

  if (ImGui::Begin("Scene graph", m_open))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);

    if (scene.getRoot().isValid())
      renderEntityEntryRecursive(scene.getRoot());

    ImGui::PopStyleVar();
  }
  ImGui::End();

  handleFrameContext();

  return ret;
}

void SceneGraph::renderEntityEntryRecursive(Entity& e)
{
  const auto& name = e.getName();
  auto& children = e.getChildren();
  const bool isLeaf = children.empty();
  const bool isNode = !isLeaf;

  ImGui::PushID(name.c_str());

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth
    // | ImGuiTreeNodeFlags_FramePadding
    ;

  if (isLeaf)
    flags =
    flags
    | ImGuiTreeNodeFlags_Leaf
    | ImGuiTreeNodeFlags_NoTreePushOnOpen
    ;
  else // isNode
    flags =
    flags
    | ImGuiTreeNodeFlags_DefaultOpen
    | ImGuiTreeNodeFlags_OpenOnArrow
    ;

  if (m_frameContext.entityEditor->isEditingEntity(e))
    flags = flags | ImGuiTreeNodeFlags_Selected;

  bool open = ImGui::TreeNodeEx(name.c_str(), flags, "%s", name.c_str());

  clickBehaviour(e);
  contextualMenuBehaviour(e);
  dragAndDropBehaviour(e);

  if (open)
  {
    for (auto& child : children)
    {
      renderEntityEntryRecursive(child);
    }

    if (isNode)
      ImGui::TreePop();
  }

  ImGui::PopID();
}

void SceneGraph::clickBehaviour(Entity& e)
{
  if (ImGui::IsItemClicked())
  {
    m_frameContext.entityEditor->openOrCloseIfSame(e);
  }
}

void SceneGraph::contextualMenuBehaviour(Entity& e)
{
  bool isRoot = e.isRoot();
  if (ImGui::BeginPopupContextItem())
  {
    if (isRoot)
      ImGui::BeginDisabled();

    if (ImGui::Selectable("Delete"))
    {
      m_frameContext.deletedEntity = e.clone();
    }

    if (isRoot)
      ImGui::EndDisabled();

    if (ImGui::Selectable("Add child"))
    {
      m_frameContext.requestNewChild = e.clone();
    }

    ImGui::EndPopup();
  }
}

void SceneGraph::dragAndDropBehaviour(Entity& e)
{
  static constexpr std::string_view payloadName = "VRM_SceneGraph_Hierarchy_Edit";

  bool validTarget = true;

  if (const ImGuiPayload* payload = ImGui::GetDragDropPayload())
  {
    if (payload->IsDataType(payloadName.data()))
    {
      Entity* newChild = (Entity*)payload->Data;
      validTarget = !m_frameContext.activeScene->checkEntityAncestor(*newChild, e);
    }
  }

  if (validTarget && ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName.data()))
    {
      Entity* newChild = (Entity*)payload->Data;

      m_frameContext.RequestHierarchyEdit.parent = e.clone();
      m_frameContext.RequestHierarchyEdit.child = newChild->clone();
    }

    ImGui::EndDragDropTarget();
  }

  // Root entity can't be move in hierarchy
  // so we do not set it as a source
  if (!e.isRoot() && ImGui::BeginDragDropSource())
  {
    ImGui::SetDragDropPayload(payloadName.data(), &e, sizeof(Entity));
    ImGui::Text("%s", e.getName().c_str());

    ImGui::EndDragDropSource();
  }
}

void SceneGraph::selectEntity(Entity& e)
{
  m_frameContext.entityEditor->openOrCloseIfSame(e);
}

void SceneGraph::unselectEntity()
{
  m_frameContext.entityEditor->close();
}

void SceneGraph::setupFrameContext()
{
  m_frameContext = {};
  m_frameContext.activeScene = &Application::Get().getGameLayer().getScene();
  m_frameContext.entityEditor = &static_cast<EntityEditor&>(UserInterfaceLayer::Get().getElement(EInterfaceElement::eEntityEditor));
}

void SceneGraph::handleFrameContext()
{
  auto& hierarchy = m_frameContext.RequestHierarchyEdit;
  const bool changeHierarchyRelation = true
    && hierarchy.child.isValid()
    && !hierarchy.child.isRoot()
    && hierarchy.parent.isValid()
    && !m_frameContext.activeScene->checkEntitiesRelation(hierarchy.parent, hierarchy.child)
    ;

  if (changeHierarchyRelation)
  {
    m_frameContext.activeScene->setEntitiesRelation(
      hierarchy.parent,
      hierarchy.child
    );
  }

  if (Entity& e = m_frameContext.deletedEntity)
  {
    m_frameContext.activeScene->destroyEntity(e);
  }

  if (Entity& parent = m_frameContext.requestNewChild)
  {
    Entity newChild = m_frameContext.activeScene->createEntity();
    m_frameContext.activeScene->setEntitiesRelation(parent, newChild);
  }
}
