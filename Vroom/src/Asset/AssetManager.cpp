#include "Vroom/Asset/AssetManager.h"

#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

namespace Vroom
{
	AssetManager::AssetManager(const uint64_t& cacheSize, const uint64_t& cacheSoftSize)
		: m_CacheMaxSize(cacheSize), m_SoftCacheMaxSize(cacheSoftSize), m_CacheSize(0)
	{

	}

	AssetManager::~AssetManager()
	{
		for (const auto& assetName : m_UnusedAssets)
		{
			m_Cache.erase(assetName);
		}

		m_UnusedAssets.clear();
	}

	void AssetManager::notifyUsedAsset(const std::string& assetPath)
	{
		//if (m_Cache.at(assetPath).second->hasInstances()) return;

		m_UnusedAssets.erase(m_Cache.at(assetPath).first);
		m_Cache.at(assetPath).first = m_UnusedAssets.end();
	}

	void AssetManager::notifyUnusedAsset(const std::string& assetPath)
	{
		m_UnusedAssets.push_back(assetPath);
		m_Cache.at(assetPath).first = std::prev(m_UnusedAssets.end());
		cleanCache();
	}

	void AssetManager::cleanCache(const size_t& assetToLoad)
	{
		while (m_CacheSize + assetToLoad > m_CacheMaxSize)
		{
			bool val = unloadOneAsset();
			VR_ASSERT_MSG(val, "Couldn't unload enough assets to make space in the assets cache.");
		}

		// We unload as much assets as we can to free enough space to reach the soft cache size.
		while (m_CacheSize + assetToLoad > m_SoftCacheMaxSize && unloadOneAsset());
	}

	bool AssetManager::unloadOneAsset()
	{
		if (m_UnusedAssets.size() == 0) return false;

		std::string unused_asset = m_UnusedAssets.front();
		VR_ASSERT_MSG(m_Cache.contains(unused_asset), "Tried to unload an unused asset : {}, but the asset wasn't in the cache.", unused_asset);
		m_CacheSize -= getAssetSize(unused_asset);
		m_UnusedAssets.pop_front();
		m_Cache.erase(unused_asset);

		return true;
	}

	uint64_t AssetManager::getAssetSize(const std::string& path)
	{
		// Will throw an exception if the file does not exist
		uint64_t size = std::filesystem::file_size(path);

		LOG_TRACE("Asset size on disk : {}", size);

		return size;
	}
}