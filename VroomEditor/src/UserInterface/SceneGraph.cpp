#include "VroomEditor/UserInterface/SceneGraph.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/HierarchyComponent.h>
#include <Vroom/Scene/Components/NameComponent.h>

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

  auto& scene = Application::Get().getGameLayer().getScene();
  
  if (ImGui::Begin("Scene graph"))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
    
    if (scene.getRoot().isValid())
      renderEntityEntryRecursive(scene.getRoot());

    ImGui::PopStyleVar();
  }
  ImGui::End();

  m_entityEditor.renderImgui();

  return ret;
}

void SceneGraph::renderEntityEntryRecursive(Entity& e)
{
  const auto& nc = e.getComponent<NameComponent>();
  HierarchyComponent& component = e.getComponent<HierarchyComponent>();
  auto& children = e.getComponent<HierarchyComponent>().children;
  const bool isLeaf = children.empty();
  const bool isNode = !isLeaf;
  
  ImGui::PushID(nc.name.c_str());

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
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

  if (m_entityEditor.isEditingEntity(e))
    flags = flags | ImGuiTreeNodeFlags_Selected;

  if (ImGui::TreeNodeEx(nc.name.c_str(), flags))
  {
    if (ImGui::IsItemClicked())
    {
      m_entityEditor.openOrCloseIfSame(e);
    }

    for (auto& child : children)
    {
      renderEntityEntryRecursive(child);
    }

    if (isNode)
      ImGui::TreePop();
  }

  ImGui::PopID();
}
