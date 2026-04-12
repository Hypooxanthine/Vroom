#include "Renderer/RenderPassManager.h"

#include "Core/Assert.h"
#include "Core/Profiling.h"

using namespace vrm;

RenderPassManager::RenderPassManager()
{}

RenderPassManager::~RenderPassManager()
{}

RenderPass& RenderPassManager::pushPass(std::unique_ptr<RenderPass>&& pass)
{
  VRM_ASSERT_MSG(m_initialized == false,
                 "RenderPassManager has been initialized. You must reset it "
                 "before pushing passes again");
  auto& pushedPass = m_passes.emplace_back(std::move(pass));
  pushedPass->managerInterface().setPassIndex(m_passes.size() - 1);
  pushedPass->managerInterface().setPassMaterials(&m_materials);

  return *m_passes.back();
}

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

  for (auto& pass : m_passes)
  {
    pass->init();
  }

  m_initialized = true;
}

void RenderPassManager::setup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("RenderPassManager::setup");

  for (auto& pass : m_passes)
  {
    pass->setup(ctx);
  }
}

void RenderPassManager::render(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("RenderPassManager::render");

  for (auto& pass : m_passes)
  {
    pass->render(ctx);
  }
}

void RenderPassManager::cleanup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("RenderPassManager::cleanup");

  for (auto& pass : m_passes)
  {
    pass->cleanup(ctx);
  }
}
