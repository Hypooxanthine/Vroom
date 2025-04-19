#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

std::unique_ptr<AssetManager> AssetManager::s_Instance = nullptr;

void AssetManager::Init()
{
    VRM_ASSERT_MSG(s_Instance == nullptr, "AssetManager already initialized.");

    auto* privateAssetManager = new AssetManager();
    s_Instance = std::unique_ptr<AssetManager>(privateAssetManager);
}

void AssetManager::Shutdown()
{
    s_Instance = nullptr;
}

AssetManager& AssetManager::Get()
{
    VRM_ASSERT_MSG(s_Instance != nullptr, "AssetManager not initialized.");
    return *s_Instance;
}

} // namespace vrm
