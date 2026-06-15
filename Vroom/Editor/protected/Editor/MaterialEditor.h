#pragma once

#include "imgui.h"

#include "AssetManager/MaterialAsset.h"
#include "AssetManager/MaterialData.h"
#include "Editor/ImguiElement.h"
#include "Editor/MaterialDataExtractor.h"


namespace vrm
{

class MaterialEditor : public ImGuiElement
{
public:

  MaterialEditor();
  ~MaterialEditor() override;

  void open(MaterialAsset::Handle material);

protected:

  void onUpdate(const DeltaTime& dt) override;
  void onRender() override;
  void onImgui() override;

private:

  void _showShadingModelTree();
  void _showPhongTree();
  void _showPBRTree();
  void _showCustomShaderTree();
  void _showPostProcessMaterialTree();

  void _showToolbar();

  void _save();
  void _apply();

  ImGuiTreeNodeFlags _getTreeNodeFlags(bool isNode) const;

public:

  MaterialAsset::Handle m_material = {};
  MaterialData m_data;
  MaterialDataExtractor m_dataExtractor;
  // There are unsaved changes in m_data (compared to the file on disk).
  bool m_edited = false;
  // The file on disk has been saved since the last apply, so reloading the
  // asset (apply) would have an effect. Drives the "Apply" button.
  bool m_savedSinceApply = false;
};

} // namespace vrm
