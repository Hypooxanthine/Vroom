#include "Editor/MeshComponentEditor.h"

#include <AssetManager/MeshAsset.h>

#include "AssetManager/MaterialAsset.h"
#include "Editor/AssetSelector.h"

#include "imgui.h"

using namespace vrm;

void MeshComponentEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  MeshSelector meshSelector(component.getMesh());
  meshSelector.renderImgui();
  if (meshSelector.getChanged())
    component.setMesh(meshSelector.getAsset());

  const size_t matSlotMax = component.getMaterials().getSlotCount();
  for (size_t matSlot = 0; matSlot < matSlotMax; ++matSlot)
  {
    MaterialAsset::Handle mat = component.getMaterials().getMaterial(matSlot);
    if (!mat.isValid())
      continue;

    MaterialSelector materialSelector(mat);
    ImGui::PushID(matSlot);
    materialSelector.renderImgui();
    ImGui::PopID();
    if (materialSelector.getChanged())
    {
      component.setMaterial(matSlot, materialSelector.getAsset());
    }
  }

  bool visible = component.isVisible();
  if (ImGui::Checkbox("Visible", &visible))
    component.setVisible(visible);

  bool castsShadow = component.doesCastShadow();
  if (ImGui::Checkbox("Casts shadow", &castsShadow))
    component.setCastsShadow(castsShadow);
}
