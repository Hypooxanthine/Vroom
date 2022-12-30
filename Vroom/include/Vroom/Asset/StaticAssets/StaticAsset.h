#pragma once

#include <string>
#include <memory>

#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

namespace Vroom
{
	class AssetManager;
	class DynamicAsset;

	class StaticAsset
	{
		friend class DynamicAsset;
		friend class AssetManager;

	public:
		StaticAsset() = delete;
		StaticAsset(const StaticAsset& other) = delete;

		virtual ~StaticAsset();

		template <typename T>
		inline std::unique_ptr<T> instanciate() const
		{
			static_assert(std::is_base_of<DynamicAsset, T>::value);

			notifyNewInstance();
			auto instance = onInstanciate();
			instance->setParent(this);

			return std::unique_ptr<T>(dynamic_cast<T*>(instance));
		}

		inline bool hasInstances() const { return m_Instances > 0; }
		inline const std::string& getPath() const { return m_Path; }
		inline bool isModified() const { return m_Modified; }

		//virtual size_t getGPUSize() const = 0;
		//virtual size_t getCPUSize() const = 0;

	protected:
		StaticAsset(AssetManager* manager, const std::string& path);
		inline void tagModified() { m_Modified = true; }
		inline AssetManager& getAssetManager() { return *m_Manager; }

	private:
		void notifyNewInstance() const;
		void notifyDeletedInstance() const;

		virtual DynamicAsset* onInstanciate() const = 0;

		inline bool load() { return onLoad(); }

		virtual bool onLoad() = 0;

	private:
		AssetManager* m_Manager;

		std::string m_Path;

		mutable size_t m_Instances = 0;

		bool m_Modified = false;
	};
}