#pragma once

#include <vector>
#include <memory>

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/PassMaterials.h"

namespace vrm
{
  
  class RenderPassManager
  {
  public:

    RenderPassManager();
    ~RenderPassManager();

    template <typename PassType, typename... Args>
    inline PassType& pushPass(Args&&... args)
    {
      VRM_ASSERT_MSG(m_initialized == false, "RenderPassManager has been initialized. You must reset it before pushing passes again");
      auto& pass = m_passes.emplace_back(std::make_unique<PassType>(std::forward<Args>(args)...));
      pass->managerInterface().setPassIndex(m_passes.size() - 1);
      pass->managerInterface().setPassMaterials(&m_materials);

      return static_cast<PassType&>(*m_passes.back());
    }

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