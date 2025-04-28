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
  m_passes.clear();
  m_initialized = false;
}

void RenderPassManager::init()
{
  VRM_ASSERT_MSG(m_initialized == false, "RenderPassManager has already been initialized");

  for (auto& p : m_passes)
  {
    p->init();
  }

  m_initialized = true;
}

void RenderPassManager::setup(const RenderPassContext& ctx)
{
  for (auto& p : m_passes)
  {
    p->setup(ctx);
  }
}

void RenderPassManager::render(const RenderPassContext& ctx) const
{
  for (const auto& p : m_passes)
  {
    p->render(ctx);
  }
}

void RenderPassManager::cleanup(const RenderPassContext& ctx)
{
  for (auto& p : m_passes)
  {
    p->cleanup(ctx);
  }
}
