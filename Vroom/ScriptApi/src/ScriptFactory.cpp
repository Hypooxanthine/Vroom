#include "ScriptApi/ScriptFactory.h"
#include "Scene/ScriptComponent.h"

using namespace vrm;

ScriptComponent* ScriptFactory::create() const
{
  ScriptComponent* rawPtr = delegateCreate();
  rawPtr->m_scriptName = m_name;

  return rawPtr;
}