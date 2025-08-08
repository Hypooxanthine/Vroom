#include "Vroom/Render/Passes/RenderPassManager.h"

using namespace vrm;

RenderPassManager::RenderPassManager()
{

}

RenderPassManager::~RenderPassManager()
{

}

void RenderPassManager::reset()
{
  for (auto& passes : m_passContainers)
    passes.clear();
  m_materials.clear();
  m_initialized = false;
  m_passCount = 0;
}

void RenderPassManager::init()
{
  VRM_ASSERT_MSG(m_initialized == false, "RenderPassManager has already been initialized");

  for (auto& passes : m_passContainers)
    for (auto& pass : passes)
    {
      pass->init();
    }

  m_initialized = true;
}

void RenderPassManager::setup(const RenderPassContext& ctx)
{
  for (auto& passes : m_passContainers)
    for (auto& pass : passes)
    {
      pass->setup(ctx);
    }
}

void RenderPassManager::render(const RenderPassContext& ctx) const
{
  for (auto& passes : m_passContainers)
    for (auto& pass : passes)
    {
      pass->render(ctx);
    }
}

void RenderPassManager::cleanup(const RenderPassContext& ctx)
{
  for (auto& passes : m_passContainers)
    for (auto& pass : passes)
    {
      pass->cleanup(ctx);
    }
}
