#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"

#include <imgui.h>

#include "VroomEditor/EditorLayer.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetParentDir.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

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
  
  std::vector<std::unique_ptr<vrm::AssetElement>> dirs;
  std::vector<std::unique_ptr<vrm::AssetElement>> files;

  for (const auto &entry : std::filesystem::directory_iterator(m_CurrentPath))
  {
    auto p = entry.path();
    p = p.lexically_relative(m_ResourcesPath.parent_path());
    // VRM_LOG_TRACE("Element: {}", p.string());
    auto element = AssetUtils::CreateAssetElement(p);

    if (element == nullptr)
      continue;

    if (entry.is_directory())
    {
      dirs.emplace_back(std::move(element));
    }
    else
    {
      files.emplace_back(std::move(element));
    }
  }

  m_Assets.reserve(m_Assets.size() + dirs.size() + files.size());
  for (auto&& elem : dirs)
    m_Assets.emplace_back(std::move(elem));
  for (auto&& elem : files)
    m_Assets.emplace_back(std::move(elem));
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

    if (ImGui::BeginPopupContextWindow("##browserpopup", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
      if (ImGui::Selectable("Open in OS explorer"))
      {
        AssetUtils::OpenNativeFileExplorer(m_CurrentPath);
      }

      ImGui::EndPopup();
    }
  }

  ImGui::End();

  setCurrentPath(nextPath);
  
  return ret;
}
