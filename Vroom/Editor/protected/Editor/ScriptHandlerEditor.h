#pragma once

#include "Editor/ComponentEditor.h"

#include <Scene/ScriptHandler.h>

namespace vrm
{

class ScriptHandlerEditor : public TComponentEditor<ScriptHandler, true>
{
public:

  std::string getComponentName() const override { return "Script component"; }

  void editEntityComponent(Entity& e) const override;
};

} // namespace vrm
