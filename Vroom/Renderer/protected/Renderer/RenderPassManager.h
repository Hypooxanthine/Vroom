#pragma once

#include <vector>
#include <memory>

#include "Renderer/RenderPass.h"
#include "Renderer/PassMaterials.h"

namespace vrm
{
  
  class RenderPassManager
  {
  public:

    RenderPassManager();
    ~RenderPassManager();
    
    RenderPassManager& operator=(const RenderPassManager& other) = delete;
    RenderPassManager(const RenderPassManager& other) = delete;
    
    RenderPassManager& operator=(RenderPassManager&& other) = default;
    RenderPassManager(RenderPassManager&& other) = default;

    template <typename PassType, typename... Args>
    inline PassType& pushPass(Args&&... args)
    {
      auto& pass = pushPass(std::make_unique<PassType>(std::forward<Args>(args)...));
      return static_cast<PassType&>(*m_passes.back());
    }

    RenderPass& pushPass(std::unique_ptr<RenderPass>&& pass);

    void reset();

    void init();

    void setup(const RenderPassContext& ctx);

    void render(const RenderPassContext& ctx) const;

    void cleanup(const RenderPassContext& ctx);

  private:

    bool m_initialized = false;
    std::vector<std::unique_ptr<RenderPass>> m_passes;
    PassMaterials m_materials;

  };

} // namespace vrm