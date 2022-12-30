#include "Vroom/Asset/StaticAssets/StaticAsset.h"

#include "Vroom/Asset/AssetManager.h"

namespace Vroom
{
	StaticAsset::StaticAsset(AssetManager* manager, const std::string& path)
		: m_Manager(manager), m_Path(path)
	{

	}

	StaticAsset::~StaticAsset()
	{

	}

	void StaticAsset::notifyNewInstance() const
	{
		if (m_Instances == 0)
			m_Manager->notifyUsedAsset(getPath());
		m_Instances++;
	}

	void StaticAsset::notifyDeletedInstance() const
	{
		m_Instances--;

		if (m_Instances == 0)
		{
			m_Manager->notifyUnusedAsset(getPath());
		}
	}
}