#include "Vroom/Render/Passes/LightClusteringPass.h"

#include "Vroom/Render/Clustering/ClusteredLights.h"
#include "Vroom/Render/Clustering/LightRegistry.h"

using namespace vrm;

LightClusteringPass::LightClusteringPass()
  : RenderPass()
{

}

LightClusteringPass::~LightClusteringPass()
{

}

void LightClusteringPass::onRender(const RenderPassContext& ctx) const
{
  VRM_ASSERT_MSG(clusteredLights != nullptr, "Invalid clustered lights");
  VRM_ASSERT_MSG(lightsStorageBuffer != nullptr, "Invalid lightsStorageBuffer");
  VRM_ASSERT_MSG(ctx.mainCamera, "Invalid camera");

  clusteredLights->setupClusters(clusterCount, *ctx.mainCamera);
  clusteredLights->processLights(*ctx.mainCamera, *lightsStorageBuffer);
}
