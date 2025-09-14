#include "Vroom/Render/Passes/ClearTexturePass.h"

#include "Vroom/Core/Profiling.h"

using namespace vrm;

ClearTexturePass::ClearTexturePass() {}

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
