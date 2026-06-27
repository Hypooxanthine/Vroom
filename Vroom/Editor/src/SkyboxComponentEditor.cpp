#include "Editor/SkyboxComponentEditor.h"

#include <AssetManager/CubemapAsset.h>

#include "Editor/AssetSelector.h"

using namespace vrm;

void SkyboxComponentEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  CubemapSelector selector(component.getCubemapAsset());
  selector.renderImgui();
  if (selector.getChanged())
  {
    component.setCubemapAsset(selector.getAsset());
  }
}
