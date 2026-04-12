#pragma once

#include <glm/glm.hpp>

#include "AssetManager/MaterialAsset.h"
#include "RenderObjects/fwds.h"
#include "Renderer/MeshTag.h"
#include "Tools/Registry.h"

namespace vrm
{

class RenderMesh;

struct MeshRenderInfo
{
  size_t                renderMeshId;
  const render::Mesh*   mesh;
  MaterialAsset::Handle material;
  const glm::mat4*      model;
  size_t                entityId;
  MeshTags              tags;
};

class MeshRegistry : public Registry<MeshRenderInfo, size_t>
{};

} // namespace vrm
