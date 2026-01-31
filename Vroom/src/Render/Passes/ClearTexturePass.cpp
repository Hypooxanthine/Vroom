#include "Vroom/Render/Passes/ClearTexturePass.h"

#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

ClearTexturePass::ClearTexturePass(const std::string& name) : RenderPass(name) {}

ClearTexturePass::~ClearTexturePass() {}

void ClearTexturePass::onInit()
{
  VRM_ASSERT(texture);
  VRM_ASSERT(format);
  VRM_ASSERT(type);
}

void ClearTexturePass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("ClearTexturePass::onRender");

  glClearTexImage(texture, level, format, type, clearValue);
}
