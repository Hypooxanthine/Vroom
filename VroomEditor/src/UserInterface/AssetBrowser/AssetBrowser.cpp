#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"

#include <imgui.h>

#include "VroomEditor/EditorLayer.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetDirectory.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetParentDir.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileSceneAsset.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

AssetFile *AssetBrowser::CreateAssetFile(const std::filesystem::path &path)
{
  if (path.extension() == ".scene")
    return new AssetFileSceneAsset(path);
  else
    return new AssetFile(path);
}

AssetBrowser::AssetBrowser(const std::filesystem::path &resourcesPath)
    : ImGuiElement(),
      m_ResourcesPath(std::filesystem::canonical(resourcesPath)),
      m_CurrentPath(m_ResourcesPath)
{
  updateDirectoryContent();
}

AssetBrowser::~AssetBrowser()
{
}

static bool IsChildOf(const std::filesystem::path &parent, const std::filesystem::path &child)
{
  std::filesystem::path relative = child.lexically_relative(parent);
  return !relative.empty() && relative.native()[0] != '.';
}

void AssetBrowser::setCurrentPath(const std::filesystem::path &newPath)
{
  auto p = std::filesystem::canonical(newPath);

  if (p != m_CurrentPath && std::filesystem::is_directory(p) && (p == m_ResourcesPath || IsChildOf(m_ResourcesPath, p)))
  {
    m_CurrentPath = p;
    updateDirectoryContent();
  }
}

void AssetBrowser::updateDirectoryContent()
{
  m_Assets.clear();

  if (m_CurrentPath != m_ResourcesPath)
  {
    m_Assets.emplace_back().reset(new AssetParentDir(m_CurrentPath.parent_path()));
  }

  // Directories first
  for (const auto &entry : std::filesystem::directory_iterator(m_CurrentPath))
  {
    if (entry.is_directory())
    {
      m_Assets.emplace_back().reset(new AssetDirectory(entry.path()));
    }
  }

  // Files then
  for (const auto &entry : std::filesystem::directory_iterator(m_CurrentPath))
  {
    if (entry.is_regular_file())
    {
      m_Assets.emplace_back().reset(CreateAssetFile(entry.path()));
    }
  }
}

bool AssetBrowser::onImgui()
{
  bool ret = false;
  m_Action = AssetElement::EAction::eNone;
  m_SelectedAsset.clear();

  std::filesystem::path nextPath = m_CurrentPath;

  if (ImGui::Begin("Asset browser"))
  {
    static constexpr char sep = std::filesystem::path::preferred_separator;
    std::string displayPath = sep + m_CurrentPath.lexically_relative(m_ResourcesPath.parent_path()).string();
    if (!displayPath.ends_with(sep))
      displayPath += sep;

    ImGui::PushID("Path");
    ImGui::TextWrapped("%s", displayPath.c_str());
    ImGui::PopID();

    bool first = true;
    float windowWidth = ImGui::GetContentRegionAvail().x;

    for (auto &elem : m_Assets)
    {
      if (first)
        first = false;
      else
      {
        ImGui::SameLine();
      }

      if (ImGui::GetCursorPosX() + AssetElement::GetElementSize().x > windowWidth)
        ImGui::NewLine();

      if (elem->renderImgui())
      {
        m_Action = elem->getAction();

        if (m_Action == AssetElement::EAction::eNavigate)
        {
          nextPath = elem->getPath();
        }
        else if (m_Action == AssetElement::EAction::eLoadScene)
        {
          EditorLayer::Get().loadScene(elem->getPath().string());
        }
        else
        {
          m_SelectedAsset = elem->getPath();
          ret = true;
        }
      }
    }
    ImGui::End();

    setCurrentPath(nextPath);
  }
  
  return ret;
}
