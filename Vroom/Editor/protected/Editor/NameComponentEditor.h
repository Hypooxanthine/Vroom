#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/NameComponent.h>

namespace vrm
{

class NameComponentEditor : public TComponentEditor<NameComponent, false>
{
public:

  std::string getComponentName() const override { return "Name tag"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
