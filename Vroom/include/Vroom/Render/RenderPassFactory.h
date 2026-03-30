#pragma once

#include <memory>

#include "Passes/RenderPass.h"

namespace vrm
{

class RenderPassFactory
{
public:

  RenderPassFactory()          = default;
  virtual ~RenderPassFactory() = default;

  virtual std::unique_ptr<RenderPass> createPass() = 0;
};

template <typename T>
class DefaultRenderPassFactory : public RenderPassFactory
{
public:

  DefaultRenderPassFactory()  = default;
  ~DefaultRenderPassFactory() = default;

  std::unique_ptr<RenderPass> createPass() override
  {
    return std::make_unique<T>();
  }
};

} // namespace vrm
