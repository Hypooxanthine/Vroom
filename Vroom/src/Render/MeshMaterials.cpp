#include "Vroom/Render/MeshMaterials.h"

using namespace vrm;

MeshMaterials::MeshMaterials()
{

}

MeshMaterials::~MeshMaterials()
{

}

void MeshMaterials::clear(size_t slots)
{
  m_materials.clear();
  m_materials.resize(slots);
}

size_t MeshMaterials::getSlotCount() const
{
  return m_materials.size();
}

size_t MeshMaterials::pushMaterial(MaterialAsset::Handle mat)
{
  m_materials.push_back(mat);

  return m_materials.size() - 1;
}

void MeshMaterials::setMaterial(size_t slot, MaterialAsset::Handle mat)
{
  VRM_CHECK_RET_MSG(getSlotCount() > slot, "Could not set material on slot {}: not enough slots", slot);

  m_materials.at(slot) = mat;
}

MaterialAsset::Handle MeshMaterials::getMaterial(size_t slot) const
{
  VRM_CHECK_RETVAL_MSG(getSlotCount() > slot, {}, "Could not get material on slot {}: not enough slots", slot);

  return m_materials.at(slot);
}
