#pragma once

#include <vector>
#include <array>
#include <memory>

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/PassMaterials.h"

namespace vrm
{
  
  class RenderPassManager
  {
  public:

    using PassContainer = std::vector<std::unique_ptr<RenderPass>>;

  public:

    RenderPassManager();
    ~RenderPassManager();

    template <typename PassType, typename... Args>
    inline PassType& pushPreCameraPass(Args&&... args)
    {
      return _pushPass<PassType>(m_passContainers.at(EPassSection::ePreCamera), std::forward<Args>(args)...);
    }

    template <typename PassType, typename... Args>
    inline PassType& pushCameraPass(Args&&... args)
    {
      return _pushPass<PassType>(m_passContainers.at(EPassSection::eCamera), std::forward<Args>(args)...);
    }

    template <typename PassType, typename... Args>
    inline PassType& pushPostCameraPass(Args&&... args)
    {
      return _pushPass<PassType>(m_passContainers.at(EPassSection::ePostCamera), std::forward<Args>(args)...);
    }

    void reset();

    void init();

    void setup(const RenderPassContext& ctx);

    void render(const RenderPassContext& ctx) const;

    void cleanup(const RenderPassContext& ctx);

  private:

    struct EPassSection
    {
      enum Type : size_t
      {
        ePreCamera = 0,
        eCamera,
        ePostCamera,
        eCount
      };
    };

  private:

    template <typename PassType, typename... Args>
    inline PassType& _pushPass(PassContainer& container, Args&&... args)
    {
      VRM_ASSERT_MSG(m_initialized == false, "RenderPassManager has been initialized. You must reset it before pushing passes again");
      auto& pass = container.emplace_back(std::make_unique<PassType>(std::forward<Args>(args)...));
      pass->managerInterface().setPassIndex(m_passCount);
      ++m_passCount;
      pass->managerInterface().setPassMaterials(&m_materials);

      return static_cast<PassType&>(*container.back());
    }

  private:

    bool m_initialized = false;
    size_t m_passCount = 0;
    std::array<PassContainer, EPassSection::eCount> m_passContainers;
    PassMaterials m_materials;

  };

} // namespace vrm
