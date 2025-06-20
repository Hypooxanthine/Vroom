#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

std::unique_ptr<AssetManager> AssetManager::s_Instance = nullptr;

AssetManager::~AssetManager()
{
    m_Assets.clear();
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

} // namespace vrm
