#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/DirectionalLightComponent.h>

namespace vrm
{

class DirectionalLightComponentEditor : public TComponentEditor<DirectionalLightComponent, true>
{
public:

  std::string getComponentName() const override { return "Directional light component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
