#include "Vroom/Render/PassMaterial.h"

#include "Vroom/Render/MaterialDefines.h"

using namespace vrm;

PassMaterial::PassMaterial()
{
  
}

PassMaterial::~PassMaterial()
{

}

void PassMaterial::setMaterial(MaterialAsset::Handle material)
{
  m_materialAsset = material;
  m_needsPrepare = true;
}

void PassMaterial::prepare(const MaterialDefines& defines)
{
  // m_materialAsset->get

  m_needsPrepare = false;
}
