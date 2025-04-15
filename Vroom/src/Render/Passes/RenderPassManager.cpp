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

void RenderPassManager::setup()
{
  for (auto& p : m_passes)
  {
    p->setup();
  }
}

void RenderPassManager::render() const
{
  for (const auto& p : m_passes)
  {
    p->render();
  }
}

void RenderPassManager::cleanup()
{
  for (auto& p : m_passes)
  {
    p->cleanup();
  }
}
