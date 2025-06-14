#pragma once

#include <vector>

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm
{

  class VRM_API MeshMaterials
  {
  public:

    MeshMaterials();
    ~MeshMaterials();

    void clear(size_t slots = 0);

    size_t getSlotCount() const;

    size_t pushMaterial(MaterialAsset::Handle mat);

    void setMaterial(size_t slot, MaterialAsset::Handle mat);
    MaterialAsset::Handle getMaterial(size_t slot) const;

  private:
    std::vector<MaterialAsset::Handle> m_materials;
  };

} // namespace vrm
