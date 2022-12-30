#pragma once

#include <list>
#include <unordered_map>
#include <filesystem>

#include "Vroom/Core/Minimal.h"

#include "Vroom/Asset/StaticAssets/StaticAsset.h"
#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

#include "Vroom/Asset/DynamicAssets/Sprite.h"
#include "Vroom/Asset/StaticAssets/SpriteAsset.h"
#include "Vroom/Asset/DynamicAssets/Texture.h"
#include "Vroom/Asset/StaticAssets/TextureAsset.h"

namespace Vroom
{
	class StaticAsset;

	class AssetManager
	{
		friend StaticAsset;

	public:
		AssetManager(const uint64_t& cacheSize, const uint64_t& cacheSoftSize);
		virtual ~AssetManager();

		template <typename T>
		[[nodiscard]] std::unique_ptr<T> getAsset(const std::string& path)
		{
			
		}

		template <>
		[[nodiscard]] std::unique_ptr<Sprite> getAsset(const std::string& path)
		{
			return std::move(getStaticAsset<SpriteAsset>(path)->instanciate<Sprite>());
		}

		template <>
		[[nodiscard]] std::unique_ptr<Texture> getAsset(const std::string& path)
		{
			return std::move(getStaticAsset<TextureAsset>(path)->instanciate<Texture>());
		}

	private:

		template <typename T>
		const T* getStaticAsset(const std::string& path)
		{
			static_assert(std::is_base_of<StaticAsset, T>::value);

			// If the static asset is NOT loaded
			if (!m_Cache.contains(path) && !tryLoadAsset<T>(path))
				return nullptr;

			return dynamic_cast<T*>(m_Cache.at(path).second.get());
		}

		template <typename T>
		bool tryLoadAsset(const std::string& path)
		{
			LOG_TRACE("Loading asset : {}.", path);

			// We have to load the static asset.
			std::unique_ptr<T> asset(new T(this, path));

			uint64_t asset_size = getAssetSize(path);

			cleanCache(asset_size);

			bool value = asset->load();
			VR_ASSERT_MSG(value, "Couldn't load asset " + path + ".");

			m_UnusedAssets.push_back(path);

			m_Cache[path] = std::pair<std::list<std::string>::const_iterator, std::unique_ptr<StaticAsset>>
				(std::prev(m_UnusedAssets.end()), std::move(asset));

			LOG_TRACE("Asset added to the cache : {}.", path);

			m_CacheSize += asset_size;

			LOG_INFO("Cache size : {}/{} (soft: {}).", m_CacheSize, m_CacheMaxSize, m_SoftCacheMaxSize);

			return true;
		}


	private:
		void notifyUsedAsset(const std::string& assetPath);
		void notifyUnusedAsset(const std::string& assetPath);
		void cleanCache(const size_t& assetToLoad = 0);
		bool unloadOneAsset();
		[[nodiscard]] uint64_t getAssetSize(const std::string& path);

	private:
		uint64_t m_CacheMaxSize, m_SoftCacheMaxSize;
		uint64_t m_CacheSize;

		std::list<std::string> m_UnusedAssets;
		std::unordered_map<std::string, std::pair<std::list<std::string>::const_iterator, std::unique_ptr<StaticAsset>>> m_Cache;
	};
}