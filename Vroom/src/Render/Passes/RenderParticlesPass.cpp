#include "Vroom/Render/Passes/RenderParticlesPass.h"

#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

RenderParticlesPass::RenderParticlesPass(const std::string& name) : RenderPass(name)
{}

RenderParticlesPass::~RenderParticlesPass()
{}

void RenderParticlesPass::onInit()
{}

void RenderParticlesPass::onSetup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("RenderParticlesPass::onSetup");

  for (const auto& emitterInfo : *emitters)
  {
    emitterInfo.emitter->setupRender();
  }
}

void RenderParticlesPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("RenderParticlesPass::onRender");

  for (const auto& emitterInfo : *emitters)
  {
    emitterInfo.emitter->executeRender(ctx, emitterInfo.model);
  }
}
