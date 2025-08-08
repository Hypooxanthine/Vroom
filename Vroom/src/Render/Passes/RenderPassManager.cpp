#include "Vroom/Render/Passes/RenderPassManager.h"

#include "Vroom/Render/RenderLayout.h"

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

void RenderPassManager::setup(RenderPassContext& ctx, const RenderLayout& renderLayout)
{
  for (auto& pass : m_passContainers.at(EPassSection::ePreCamera))
  {
    pass->setup(ctx);
  }

  for (size_t row = 0; row < renderLayout.getRows(); ++row)
  {
    for (size_t col = 0; col < renderLayout.getCols(); ++col)
    {
      if (renderLayout.isViewUsed(row, col))
      {
        const RenderView& view = renderLayout.getView(row, col);
        ctx.mainCamera = view.getCamera();
        ctx.viewport = view.getViewport(ctx.framebufferSize);

        for (auto& pass : m_passContainers.at(EPassSection::eCamera))
        {
          pass->setup(ctx);
        }
      }
    }
  }

  ctx.mainCamera = nullptr;

  for (auto& pass : m_passContainers.at(EPassSection::ePostCamera))
  {
    pass->setup(ctx);
  }
}

void RenderPassManager::render(RenderPassContext& ctx, const RenderLayout& renderLayout) const
{
  for (auto& pass : m_passContainers.at(EPassSection::ePreCamera))
  {
    pass->render(ctx);
  }

  for (size_t row = 0; row < renderLayout.getRows(); ++row)
  {
    for (size_t col = 0; col < renderLayout.getCols(); ++col)
    {
      if (renderLayout.isViewUsed(row, col))
      {
        const RenderView& view = renderLayout.getView(row, col);
        ctx.mainCamera = view.getCamera();
        ctx.viewport = view.getViewport(ctx.framebufferSize);

        for (auto& pass : m_passContainers.at(EPassSection::eCamera))
        {
          pass->render(ctx);
        }
      }
    }
  }

  ctx.mainCamera = nullptr;

  for (auto& pass : m_passContainers.at(EPassSection::ePostCamera))
  {
    pass->render(ctx);
  }
}

void RenderPassManager::cleanup(RenderPassContext& ctx, const RenderLayout& renderLayout)
{
  for (auto& pass : m_passContainers.at(EPassSection::ePreCamera))
  {
    pass->cleanup(ctx);
  }

  for (size_t row = 0; row < renderLayout.getRows(); ++row)
  {
    for (size_t col = 0; col < renderLayout.getCols(); ++col)
    {
      if (renderLayout.isViewUsed(row, col))
      {
        const RenderView& view = renderLayout.getView(row, col);
        ctx.mainCamera = view.getCamera();
        ctx.viewport = view.getViewport(ctx.framebufferSize);

        for (auto& pass : m_passContainers.at(EPassSection::eCamera))
        {
          pass->cleanup(ctx);
        }
      }
    }
  }

  ctx.mainCamera = nullptr;

  for (auto& pass : m_passContainers.at(EPassSection::ePostCamera))
  {
    pass->cleanup(ctx);
  }
}
