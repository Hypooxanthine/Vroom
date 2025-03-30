#pragma once

#include <vector>
#include <string>

#include "Vroom/Render/RawShaderData/SSBOClusterInfo.h"

#include "Vroom/Render/Abstraction/StorageBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Camera/CameraBasic.h"


namespace vrm
{

  namespace gl
  {
    class StorageBufferBase;
  }

  class ClusteredLights
  {
  public:
    ClusteredLights();
    ClusteredLights(const ClusteredLights&) = default;
    ClusteredLights(ClusteredLights&&) = default;
    ~ClusteredLights() = default;

    ClusteredLights& operator=(const ClusteredLights&) = default;
    ClusteredLights& operator=(ClusteredLights&&) = default;

    void setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera);

    void processLights(const CameraBasic& camera, const gl::StorageBufferBase& lights);

    inline const gl::AutoResizeStorageBuffer& getClustersShaderStorage() const { return m_SSBOClusterInfoSSBO; }

  private:
    SSBOClusterInfo m_SSBOClusterInfoData;
    gl::AutoResizeStorageBuffer m_SSBOClusterInfoSSBO;

    glm::uvec3 m_ClusterCount;
    unsigned int m_TotalClusters;
    glm::mat4 m_Projection;

    gl::Shader m_ClustersBuilder, m_LightsCuller;
  };

} // namespace vrm
