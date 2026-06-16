#pragma once

#include <functional>

#include "imgui.h"

#include "AssetManager/MaterialAsset.h"
#include "AssetManager/MaterialData.h"
#include "Editor/ImguiElement.h"
#include "Editor/MaterialDataExtractor.h"
#include "Editor/ViewportModule.h"
#include "Renderer/FirstPersonCamera.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"


namespace vrm
{

class MaterialEditor : public ImGuiElement
{
public:

  MaterialEditor();
  ~MaterialEditor() override;

  MaterialEditor(const MaterialEditor&)            = delete;
  MaterialEditor& operator=(const MaterialEditor&) = delete;
  MaterialEditor(MaterialEditor&&)                 = delete;
  MaterialEditor& operator=(MaterialEditor&&)      = delete;

  void open(MaterialAsset::Handle material);

protected:

  void onUpdate(const DeltaTime& dt) override;
  void onRender() override;
  void onImgui() override;

private:

  // Viewport reporting its size back so the preview scene can be resized to match.
  struct PreviewViewport : public ViewportModule
  {
    std::function<void(const glm::uvec2&)> resizeCb;

  protected:
    void onResize(const ImVec2& size) override;
  };

  void _showShadingModelTree();
  void _showPhongTree();
  void _showPBRTree();
  void _showCustomShaderTree();
  void _showPostProcessMaterialTree();

  void _showToolbar();
  void _showPreview();

  void _save();
  void _apply();

  // Preview helpers
  void _setupPreviewScene();
  // Schedules a preview rebuild after the debounce delay has elapsed.
  void _markPreviewDirty();
  // Rebuilds the preview material from m_data and applies it to the sphere.
  void _rebuildPreviewMaterial();

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

private:

  /** Live preview */

  // The preview shows a sphere with m_data applied to it.
  PreviewViewport   m_previewViewport;
  Scene             m_previewScene;
  FirstPersonCamera m_previewCamera;
  Entity            m_previewEntity;

  // Material rebuilt from m_data and applied to the preview sphere. Owned here
  // (not registered in the AssetManager) and built from memory.
  MaterialAsset m_previewMaterial;

  // True once m_previewMaterial holds a valid build and is bound to the sphere.
  bool m_previewMaterialBound = false;

  // Debounce: an edit marks the preview dirty and (re)starts the timer; the
  // material is only rebuilt once no further change happened for s_previewDelay.
  bool  m_previewDirty    = false;
  float m_timeSinceChange = 0.f;

  // Slow auto-rotation so specular highlights sweep across the surface.
  float m_previewSpin = 0.f;

  static constexpr float s_previewDelay = 0.5f;
};

} // namespace vrm
