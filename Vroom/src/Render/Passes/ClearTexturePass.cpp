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
  VRM_ASSERT(clearValue);
}

void ClearTexturePass::onRender(const RenderPassContext& ctx) const
{
  glClearTexSubImage(texture,
  	0,
  	0,
  	0,
  	0,
  	width,
  	height,
  	1,
  	format,
  	type,
  	clearValue);
}
