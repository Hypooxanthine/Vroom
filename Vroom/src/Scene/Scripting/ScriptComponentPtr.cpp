#include "Vroom/Scene/Scripting/ScriptComponentPtr.h"

#include "Vroom/Scene/Scripting/RuntimeScriptLibrary.h"

using namespace vrm;

void ScriptDeleter::operator()(ScriptComponent* script) const
{
  if (owner)
    owner->_destroyScript(script);
}
