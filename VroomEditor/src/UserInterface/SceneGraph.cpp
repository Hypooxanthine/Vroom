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

  const auto& scene = Application::Get().getGameLayer().getScene();
  
  if (ImGui::Begin("Scene graph"))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
    renderEntityEntryRecursive(scene.getRoot());
    ImGui::PopStyleVar();
  }
  ImGui::End();

  return ret;
}

void SceneGraph::renderEntityEntryRecursive(const Entity& e)
{
  const auto& nc = e.getComponent<NameComponent>();
  const auto& children = e.getComponent<HierarchyComponent>().children;
  bool isLeaf = children.empty();

  
  ImGui::PushID(nc.name.c_str());

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
  if (isLeaf)
    flags = flags | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

  if (ImGui::TreeNodeEx(nc.name.c_str(), flags))
  {
    for (const auto& child : children)
    {
      renderEntityEntryRecursive(child);
    }

    if (!isLeaf)
      ImGui::TreePop();
  }

  ImGui::PopID();
}
