#pragma once

#include <filesystem>
#include <vector>

#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "BrowserNode.h"

namespace vrm
{

class AssetElement;
class AssetFile;
struct OSFileDrop;

class AssetBrowser : public ImGuiElement
{
public:

  AssetBrowser();
  ~AssetBrowser();

  void addRootPath(const std::filesystem::path& newRootPath);
  void setCurrentPath(const std::filesystem::path& newPath);
  void updateDirectoryContent();

  AssetElement* findCurrentDirElement(const std::string& elementName);
  AssetElement* findCurrentDirElement(const std::filesystem::path& elementPath);
  void          toggleSelectElement(const AssetElement* element);
  void          toggleSelectElement(const std::string& elementName);
  void          selectElement(const AssetElement* element);
  void          selectElement(const std::string& elementName);
  void          selectElement(const std::filesystem::path& elementPath);
  void          unselectElement();
  void unselectElement(const AssetElement* element); // Only unselect if the currently selected element is "element"

protected:

  void onImgui() override;

private:

  bool _isChildOf(const std::filesystem::path& child, const std::filesystem::path& parent) const;
  void _handleFileDrop(const OSFileDrop& dropData);
  void _handleContextWindow();

private:

  std::vector<BrowserNode>   m_rootNodes;
  BrowserNode m_currentNode;

  std::vector<std::unique_ptr<AssetElement>> m_Assets;
  AssetElement*                              m_selectedAsset = nullptr;
};

} // namespace vrm
