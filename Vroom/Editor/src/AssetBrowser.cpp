#include "Editor/AssetBrowser.h"
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>

#include "AssetManager/SceneData.h"
#include "AssetManager/SceneParsing.h"
#include "Core/Log.h"
#include "Editor/AssetFileScriptAsset.h"
#include "Editor/AssetParentDir.h"
#include "Editor/AssetUtils.h"
#include "Editor/BrowserNode.h"
#include "Editor/EditorLayer.h"
#include "Editor/ModelImporter.h"
#include "Editor/OSFileDrop.h"
#include "Scene/Scene.h"
#include "ScriptEngine/ScriptEngine.h"
#include "nlohmann/json_fwd.hpp"


using namespace vrm;
namespace fs = std::filesystem;

AssetBrowser::AssetBrowser() : ImGuiElement()
{
  updateDirectoryContent();
}

AssetBrowser::~AssetBrowser()
{}

void AssetBrowser::addRootPath(const std::filesystem::path& newRootPath)
{
  m_rootNodes.emplace_back(newRootPath, newRootPath);
  updateDirectoryContent();
}

void AssetBrowser::setCurrentPath(const std::filesystem::path& newPath)
{
  if (!m_currentNode.comparePath(newPath))
  {
    // Finding the root of the new path
    const BrowserNode* root = nullptr;

    if (newPath != fs::path{})
    {
      for (const BrowserNode& candidateRoot : m_rootNodes)
      {
        if (_isChildOf(newPath, candidateRoot.getPath()))
        {
          root = &candidateRoot;
          break;
        }
      }
    }

    if (root)
      m_currentNode = BrowserNode(newPath, root->getPath());
    else
      m_currentNode = BrowserNode(); // Virtual root

    updateDirectoryContent();
  }
}

void AssetBrowser::updateDirectoryContent()
{
  m_Assets.clear();
  m_selectedAsset = nullptr;

  if (m_currentNode.ok())
  {
    m_Assets.emplace_back().reset(new AssetParentDir(m_currentNode.getParent().getPath()));
  }

  std::vector<std::unique_ptr<vrm::AssetElement>> dirs;
  std::vector<std::unique_ptr<vrm::AssetElement>> files;

  std::unique_ptr<std::vector<BrowserNode>> ptrIfRootNodes;
  std::span<BrowserNode>                    nodesToIterateOn;

  if (m_currentNode.ok())
  {
    ptrIfRootNodes   = std::make_unique<std::vector<BrowserNode>>(m_currentNode.getChildren());
    nodesToIterateOn = *ptrIfRootNodes;
  }
  else
  {
    nodesToIterateOn = m_rootNodes;
  }

  for (const BrowserNode& child : nodesToIterateOn)
  {
    auto element = AssetUtils::CreateAssetElement(child.getPath());

    if (element == nullptr)
      continue;

    if (child.isDirectory())
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

  if (!m_currentNode.ok())
  {
    const auto scriptIds = ScriptEngine::Get().getRegisteredScriptIds();
    m_Assets.reserve(m_Assets.size() + scriptIds.size());

    for (const auto& scriptId : scriptIds)
    {
      std::filesystem::path virtualPath = scriptId;
      m_Assets.emplace_back(std::make_unique<AssetFileScriptAsset>(virtualPath));
    }
  }

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

  auto it = std::find_if(m_Assets.begin(), m_Assets.end(),
                         [&element](const std::unique_ptr<AssetElement>& elem)
                         {
                           return elem.get() == element;
                         });

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

  auto it = std::find_if(m_Assets.begin(), m_Assets.end(),
                         [&element](const std::unique_ptr<AssetElement>& elem)
                         {
                           return elem.get() == element;
                         });

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
    std::string           displayPath(&sep, 1);

    if (m_currentNode.ok())
      displayPath += m_currentNode.getPathFromToRoot().string();

    if (!displayPath.ends_with(sep))
      displayPath += sep;

    ImGui::PushID("Path");
    ImGui::TextWrapped("%s", displayPath.c_str());
    ImGui::PopID();

    bool  first       = true;
    float windowWidth = ImGui::GetContentRegionAvail().x;

    ImVec2 fileDropStart = ImGui::GetCursorScreenPos();

    for (auto& elem : m_Assets)
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

    if (ImGui::BeginPopupContextWindow("##browserpopup",
                                       ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
      _handleContextWindow();

      ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropTargetCustom(regionBox, ImGui::GetID("FileDropTarget")))
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OSFileDrop");
          payload && payload->DataSize == sizeof(OSFileDrop))
      {
        OSFileDrop* fileDrop = reinterpret_cast<OSFileDrop*>(payload->Data);
        _handleFileDrop(*fileDrop);
      }

      ImGui::EndDragDropTarget();
    }
  }

  ImGui::End();
}

bool AssetBrowser::_isChildOf(const std::filesystem::path& child, const std::filesystem::path& parent) const
{
  if (child == parent)
    return true;

  std::filesystem::path relative = child.lexically_relative(parent);
  return (!relative.empty() && relative.native()[0] != '.');
}

void AssetBrowser::_handleFileDrop(const OSFileDrop& dropData)
{
  VRM_ASSERT(dropData.files);
  VRM_LOG_TRACE("Dropped {}", dropData.files);

  if (!m_currentNode.ok())
  {
    // Cannot drop a file on the virtual root
    VRM_LOG_WARN("Cannot drop a file in the browser root");
    return;
  }

  std::filesystem::path from = dropData.files;
  std::filesystem::path to   = m_currentNode.getPath();
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
      std::filesystem::copy(
        from, to, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
      VRM_LOG_TRACE("Successfully copied from {} to {}", from.string(), to.string());
      updateDirectoryContent();
    }
  } catch (const std::exception& e)
  {
    VRM_LOG_WARN("Could not copy {} to {}: {}", from.string(), to.string(), e.what());
  }
}

void AssetBrowser::_handleContextWindow()
{
  if (ImGui::Selectable("Create directory"))
  {
    EditorLayer::Get().pushFrameEndRoutine(
      [this](auto& layer)
      {
        static const std::string newDirName = "NewDirectory";
        if (AssetUtils::CreateDirectory(m_currentNode.getPath() / newDirName))
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
    AssetUtils::OpenNativeFileExplorer(m_currentNode.getPath());
  }

  if (ImGui::BeginMenu("Create new..."))
  {
    if (ImGui::MenuItem("Scene"))
    {
      EditorLayer::Get().pushFrameEndRoutine(
        [this](auto& layer)
        {
          std::filesystem::path newScenePath = AssetUtils::FindFreeAssetName(m_currentNode.getPath() / "NewScene.json");
          std::ofstream         ofs(newScenePath);

          if (ofs.is_open())
          {
            nlohmann::json j = SceneData();
            ofs << j.dump(2);
            ofs.close();

            MetaFile meta;
            meta.Type = MetaFile::EType::eScene;
            AssetUtils::CreateMetaFile(meta, newScenePath);

            updateDirectoryContent();
            AssetElement* elem = findCurrentDirElement(newScenePath.filename().string());
            if (elem) // Should always be true
            {
              toggleSelectElement(elem);
              elem->setRenaming();
            }
          }
        });
    }

    ImGui::EndMenu();
  }
}
