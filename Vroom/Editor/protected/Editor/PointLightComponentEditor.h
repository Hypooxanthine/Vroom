#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/PointLightComponent.h>

namespace vrm
{

class PointLightComponentEditor : public TComponentEditor<PointLightComponent, true>
{
public:

  std::string getComponentName() const override { return "Point light component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
