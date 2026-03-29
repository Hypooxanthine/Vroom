#include "VroomEditor/UserInterface/AssetBrowser/AssetFileScriptAsset.h"

#include <imgui.h>

using namespace vrm;

AssetFileScriptAsset::AssetFileScriptAsset(const std::filesystem::path& path)
  : AssetFile(path)
{
  m_isTextEditable = false;
  m_deletable      = false;
  m_rightClickable = false;
}

AssetFileScriptAsset::~AssetFileScriptAsset()
{}

std::string AssetFileScriptAsset::getPictoAssetName() const
{
  return "Resources/Editor/Picto/file.png";
}

void AssetFileScriptAsset::onAddCustomBehaviour()
{
  if (ImGui::BeginDragDropSource())
  {
    ImGui::SetDragDropPayload("ScriptAsset", &getPath(), sizeof(getPath()));
    ImGui::Text("%s", getText().c_str());
    ImGui::EndDragDropSource();
  }
}
