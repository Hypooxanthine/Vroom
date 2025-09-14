#include "Vroom/Render/Passes/RenderPassManager.h"

#include "Vroom/Core/Profiling.h"

using namespace vrm;

RenderPassManager::RenderPassManager() {}

RenderPassManager::~RenderPassManager() {}

void RenderPassManager::reset()
{
  m_passes.clear();
  m_materials.clear();
  m_initialized = false;
}

void RenderPassManager::init()
{
  VRM_ASSERT_MSG(m_initialized == false,
                 "RenderPassManager has already been initialized");

  for (auto& pass : m_passes) { pass->init(); }

  m_initialized = true;
}

void RenderPassManager::setup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("RenderPassManager::setup");

  for (auto& pass : m_passes) { pass->setup(ctx); }
}

void RenderPassManager::render(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("RenderPassManager::render");

  for (auto& pass : m_passes) { pass->render(ctx); }
}

void RenderPassManager::cleanup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("RenderPassManager::cleanup");

  for (auto& pass : m_passes) { pass->cleanup(ctx); }
}
