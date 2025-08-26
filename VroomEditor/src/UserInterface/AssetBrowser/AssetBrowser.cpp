#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "VroomEditor/EditorLayer.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetParentDir.h"
#include "VroomEditor/UserInterface/OSFileDrop.h"
#include "VroomEditor/Import/ModelImporter.h"

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
  m_selectedAsset = nullptr;

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

  for (auto& elem : m_Assets)
    elem->setBrowser(this);
}

AssetElement* AssetBrowser::findCurrentDirElement(const std::string& elementName)
{
  for (const auto& elem : m_Assets)
  {
    if (elem->getPath().has_filename() && elem->getPath().filename() == elementName)
    {
      return elem.get();
    }
  }

  return nullptr;
}

AssetElement* AssetBrowser::findCurrentDirElement(const std::filesystem::path& elementPath)
{
  for (const auto& elem : m_Assets)
  {
    if (elem->getPath().has_filename() && elem->getPath() == elementPath)
    {
      return elem.get();
    }
  }

  return nullptr;
}

void AssetBrowser::toggleSelectElement(const AssetElement* element)
{
  if (m_selectedAsset == element)
  {
    unselectElement();
    return;
  }
  else
  {
    unselectElement();
  }

  auto it = std::find_if(m_Assets.begin(), m_Assets.end(), [&element](const std::unique_ptr<AssetElement>& elem) { return elem.get() == element;});

  if (it == m_Assets.end())
  {
    return;
  }

  it->get()->setSelected(true);
  m_selectedAsset = it->get();
}

void AssetBrowser::toggleSelectElement(const std::string& elementName)
{
  const AssetElement* elem = findCurrentDirElement(elementName);
  if (elem)
  {
    toggleSelectElement(elem);
  }
}

void AssetBrowser::selectElement(const AssetElement* element)
{
  if (m_selectedAsset == element)
  {
    return;
  }

  unselectElement(); // If any is selected, stop it

  auto it = std::find_if(m_Assets.begin(), m_Assets.end(), [&element](const std::unique_ptr<AssetElement>& elem) { return elem.get() == element;});

  if (it == m_Assets.end())
  {
    return;
  }

  it->get()->setSelected(true);
  m_selectedAsset = it->get();
}

void AssetBrowser::selectElement(const std::string& elementName)
{
  const AssetElement* elem = findCurrentDirElement(elementName);
  if (elem)
  {
    selectElement(elem);
  }
}

void AssetBrowser::selectElement(const std::filesystem::path& elementPath)
{
  const AssetElement* elem = findCurrentDirElement(elementPath);
  if (elem)
  {
    selectElement(elem);
  }
}

void AssetBrowser::unselectElement()
{
  if (m_selectedAsset)
  {
    m_selectedAsset->setSelected(false);
    m_selectedAsset = nullptr;
  }
}

void AssetBrowser::unselectElement(const AssetElement* element)
{
  if (m_selectedAsset == element)
  {
    unselectElement();
  }
}

void AssetBrowser::onImgui()
{
  if (ImGui::Begin("Asset browser", m_open))
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

    ImVec2 fileDropStart = ImGui::GetCursorScreenPos();
    

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

      elem->renderImgui();
    }

    // windowSize.y = ImGui::GetCursorPosY() - windowPos.y;
    ImRect regionBox = { fileDropStart, ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax() };

    if (ImGui::BeginPopupContextWindow("##browserpopup", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
      if (ImGui::Selectable("Create directory"))
      {
        EditorLayer::Get().pushFrameEndRoutine([this](auto& layer) {
          static const std::string newDirName = "NewDirectory";
          if (AssetUtils::CreateDirectory(m_CurrentPath / newDirName))
          {
            updateDirectoryContent();
            AssetElement* elem = findCurrentDirElement(newDirName);
            if (elem) // Should always be true
            {
              toggleSelectElement(elem);
              elem->setRenaming();
            }
          }
        });
      }

      if (ImGui::Selectable("Open in OS explorer"))
      {
        AssetUtils::OpenNativeFileExplorer(m_CurrentPath);
      }

      ImGui::EndPopup();
    }
    
    if (ImGui::BeginDragDropTargetCustom(regionBox, ImGui::GetID("FileDropTarget")))
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OSFileDrop"); payload && payload->DataSize == sizeof(OSFileDrop))
      {
        OSFileDrop* fileDrop = reinterpret_cast<OSFileDrop*>(payload->Data);
        _handleFileDrop(*fileDrop);
      }

      ImGui::EndDragDropTarget();
    }
  }

  ImGui::End();
}

void AssetBrowser::_handleFileDrop(const OSFileDrop& dropData)
{
  VRM_ASSERT(dropData.files);
  VRM_LOG_TRACE("Dropped {}", dropData.files);
  
  std::filesystem::path from = dropData.files;
  std::filesystem::path to = m_CurrentPath;
  if (from.has_filename())
  {
    if (true)
    {
      ModelImporter importer;
      if (importer.import(from, to))
      {
        VRM_LOG_INFO("Imported file {} to {} directory.", from.string(), to.string());
        updateDirectoryContent();
      }
      else
      {
        VRM_LOG_WARN("Error while importing file {} to {} directory.", from.string(), to.string());
      }
      return;
    }
    else
    {
      to = to / from.filename();
    }
  }

  try
  {
    if (!std::filesystem::exists(from))
    {
      VRM_LOG_WARN("Source path does not exist: {}", from.string());
    }
    else
    {
      std::filesystem::copy(from, to, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
      VRM_LOG_TRACE("Successfully copied from {} to {}", from.string(), to.string());
      updateDirectoryContent();
    }
  }
  catch (const std::exception& e)
  {
    VRM_LOG_WARN("Could not copy {} to {}: {}", from.string(), to.string(), e.what());
  }
}
