#pragma once

#include <vector>
#include <memory>

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Passes/RenderPass.h"

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
      m_passes.emplace_back(std::make_unique<PassType>(std::forward<Args>(args)...));

      return static_cast<PassType&>(*m_passes.back());
    }

    void reset();

    void init();

    void setup();

    void render() const;

    void cleanup();

  private:

    bool m_initialized = false;
    std::vector<std::unique_ptr<RenderPass>> m_passes;

  };

} // namespace vrm
