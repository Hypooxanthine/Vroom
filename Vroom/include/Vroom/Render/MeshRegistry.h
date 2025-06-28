#pragma once

#include <glm/glm.hpp>
#include <entt/fwd.hpp>

#include "Vroom/Render/Helpers/Registry.h"
#include "Vroom/Render/MeshTag.h"

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm
{

  class RenderMesh;

  struct MeshRenderInfo
  {
    size_t renderMeshId;
		const RenderMesh* mesh;
    MaterialAsset::Handle material;
		const glm::mat4* model;
    entt::id_type entityId;
    MeshTags tags;
  };
  
  class MeshRegistry : public Registry<MeshRenderInfo, size_t> {};

} // namespace vrm
