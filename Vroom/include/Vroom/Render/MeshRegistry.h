#pragma once

#include <glm/glm.hpp>
#include <entt/fwd.hpp>

#include "Vroom/Render/Helpers/Registry.h"

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm
{

  class RenderMesh;

  struct MeshRenderInfo
  {
		const RenderMesh* mesh;
    MaterialAsset::Handle material;
		const glm::mat4* model;
    bool visible;
    bool castsShadow;
    entt::id_type entityId;
  };
  
  class MeshRegistry : public Registry<MeshRenderInfo, size_t> {};

} // namespace vrm
