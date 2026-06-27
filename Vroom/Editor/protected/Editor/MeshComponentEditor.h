#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/MeshComponent.h>

namespace vrm
{

class MeshComponentEditor : public TComponentEditor<MeshComponent, true>
{
public:

  std::string getComponentName() const override { return "Mesh component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
