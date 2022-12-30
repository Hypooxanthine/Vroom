#include "Vroom/Asset/DynamicAssets/DynamicAsset.h"

#include "Vroom/Asset/StaticAssets/StaticAsset.h"

#include "Vroom/Core/Minimal.h"

namespace Vroom
{
	DynamicAsset::~DynamicAsset()
	{
		m_Parent->notifyDeletedInstance();
	}
}