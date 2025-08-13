#include "Vroom/Render/Passes/RenderParticlesPass.h"
#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

namespace
{

  struct RawParticleStates
  {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
  };

}

RenderParticlesPass::RenderParticlesPass()
{

}

RenderParticlesPass::~RenderParticlesPass()
{

}

void RenderParticlesPass::onInit()
{

}

void RenderParticlesPass::onSetup(const RenderPassContext &ctx)
{

}

void RenderParticlesPass::onRender(const RenderPassContext &ctx) const
{
  
}
