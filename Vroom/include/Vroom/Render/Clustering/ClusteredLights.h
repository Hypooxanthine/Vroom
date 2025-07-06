#pragma once

#include <vector>
#include <string>

#include "Vroom/Render/AutoBuffer.h"

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
    ClusteredLights(const ClusteredLights&) = delete;
    ClusteredLights(ClusteredLights&&) = default;
    ~ClusteredLights() = default;

    ClusteredLights& operator=(const ClusteredLights&) = delete;
    ClusteredLights& operator=(ClusteredLights&&) = default;

    void setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera);

    void processLights(const CameraBasic& camera, const gl::Buffer& lights);

    inline const gl::Buffer& getClustersShaderStorage() const { return m_clustersBuffer.getBuffer(); }

  private:
    render::AutoBuffer m_clustersBuffer;

    glm::uvec3 m_ClusterCount;
    unsigned int m_TotalClusters;
    glm::mat4 m_Projection;

    gl::Shader m_ClustersBuilder, m_LightsCuller;
  };

} // namespace vrm
