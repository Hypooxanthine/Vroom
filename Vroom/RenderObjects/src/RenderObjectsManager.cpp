#include "RenderObjects/RenderObjectsManager.h"
#include <cassert>
#include <memory>

using namespace vrm;
using namespace vrm::render;

namespace
{

static std::unique_ptr<RenderObjectsManager> s_manager = nullptr;

} // namesapce

void RenderObjectsManager::Init()
{
  s_manager.reset(new RenderObjectsManager);
}

void RenderObjectsManager::Shutdown()
{
  s_manager.reset();
}

RenderObjectsManager& RenderObjectsManager::Get()
{
  assert(s_manager.get() != nullptr);
  return *s_manager;
}

ShaderManager& RenderObjectsManager::getShaderManager()
{
  return m_shaderManager;
}
