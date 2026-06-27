#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/TransformComponent.h>

namespace vrm
{

class TransformComponentEditor : public TComponentEditor<TransformComponent, false>
{
public:

  std::string getComponentName() const override { return "Transform component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
