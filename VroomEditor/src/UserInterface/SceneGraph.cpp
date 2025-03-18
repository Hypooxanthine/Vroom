#include "VroomEditor/UserInterface/SceneGraph.h"

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
  
  if (ImGui::Begin("Scene graph"))
  {
    
  }
  ImGui::End();

  return ret;
}
