#include "ScriptApi/ScriptFactory.h"
#include "Scene/ScriptComponent.h"

using namespace vrm;

ScriptComponent* ScriptFactory::create() const
{
  ScriptComponent* rawPtr = delegateCreate();
  rawPtr->setScriptName(m_name);

  return rawPtr;
}