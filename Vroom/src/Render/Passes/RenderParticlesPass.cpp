#include "Vroom/Render/Passes/RenderParticlesPass.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/ParticleEmitter.h"

using namespace vrm;

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
  for (const auto& emitterInfo : *emitters)
  {
    emitterInfo.emitter->setupRender();
  }
}

void RenderParticlesPass::onRender(const RenderPassContext &ctx) const
{
  for (const auto& emitterInfo : *emitters)
  {
    emitterInfo.emitter->executeRender(ctx, emitterInfo.model);
  }
}
