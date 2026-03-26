#include "Vroom/Asset/AssetManager.h"
#include <filesystem>

using namespace vrm;

std::unique_ptr<AssetManager> AssetManager::s_Instance = nullptr;

AssetManager::AssetManager()
{
#ifdef VRM_GAME_RSC_DIR_PATH
  m_virtualDirs.addDirectory(VRM_GAME_RSC_DIR_PATH);
#endif
}

AssetManager::~AssetManager()
{
  _clear();
}

void AssetManager::Init()
{
  VRM_ASSERT_MSG(s_Instance == nullptr, "AssetManager already initialized.");

  auto* privateAssetManager = new AssetManager();
  s_Instance = std::unique_ptr<AssetManager>(privateAssetManager);
}

void AssetManager::Shutdown()
{
  if (s_Instance)
  {
    s_Instance = nullptr;
  }
}

AssetManager& AssetManager::Get()
{
  VRM_ASSERT_MSG(s_Instance != nullptr, "AssetManager not initialized.");
  return *s_Instance;
}

const VirtualDirectoriesEmulator& AssetManager::getVirtualPathEmulator() const
{
  return m_virtualDirs;
}

void AssetManager::_clear()
{
  m_keys.clear();
  for (auto rit = m_assets.rbegin(); rit != m_assets.rend(); ++rit)
  {
    rit->reset();
  }
  m_assets.clear();
}

bool AssetManager::isAssetLoaded(const std::filesystem::path& assetID)
{
  return m_keys.contains(_formatAssetId(assetID));
}

std::filesystem::path AssetManager::_resolveVirtualPath(
  const std::filesystem::path& virtualPath) const
{
  return m_virtualDirs.findPath(virtualPath);
}

std::filesystem::path
AssetManager::_formatAssetId(const std::filesystem::path& assetID) const
{
  auto normalized = assetID.lexically_normal();
  if (assetID.is_absolute())
    return normalized.lexically_relative(std::filesystem::current_path());
  else
   return normalized;
}
