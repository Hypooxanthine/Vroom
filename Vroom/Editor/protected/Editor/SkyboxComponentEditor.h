#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/SkyboxComponent.h>

namespace vrm
{

class SkyboxComponentEditor : public TComponentEditor<SkyboxComponent, true>
{
public:

  std::string getComponentName() const override { return "Skybox component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
