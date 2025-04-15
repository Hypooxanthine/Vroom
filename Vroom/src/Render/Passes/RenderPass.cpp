#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::init()
{
  onInit();
}

void RenderPass::setup()
{
  onSetup();
}

void RenderPass::render() const
{
  onRender();
}

void RenderPass::cleanup()
{
  onCleanup();
}
