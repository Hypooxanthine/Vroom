#include "Vroom/Render/Passes/ClearTexturePass.h"

using namespace vrm;


ClearTexturePass::ClearTexturePass()
{

}

ClearTexturePass::~ClearTexturePass()
{

}

void ClearTexturePass::onInit()
{
  VRM_ASSERT(texture);
  VRM_ASSERT(format);
  VRM_ASSERT(type);
}

void ClearTexturePass::onRender(const RenderPassContext& ctx) const
{
  glClearTexImage(texture,
    level,
  	format,
  	type,
  	clearValue);
}
