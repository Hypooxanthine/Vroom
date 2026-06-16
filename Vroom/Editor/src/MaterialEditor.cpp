#include "Editor/MaterialEditor.h"

#include <filesystem>
#include <misc/cpp/imgui_stdlib.h>
#include <variant>

#include <glm/glm.hpp>

#include "imgui.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/JsonFile.h"
#include "AssetManager/MaterialAsset.h"
#include "AssetManager/MaterialData.h"
#include "AssetManager/MeshAsset.h"
#include "AssetManager/TextureAsset.h"
#include "Core/DeltaTime.h"
#include "Core/Log.h"
#include "Editor/AssetSelector.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/Renderer.h"
#include "Scene/DirectionalLightComponent.h"
#include "Scene/MeshComponent.h"
#include "Scene/SkyboxComponent.h"
#include "Scene/TransformComponent.h"
#include "Tools/Utility.h"

using namespace vrm;

MaterialEditor::MaterialEditor()
  : m_previewCamera(0.1f, 100.f, glm::radians(45.f), 1.f, glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f))
{
  _setupPreviewScene();
}

MaterialEditor::~MaterialEditor()
{}

void MaterialEditor::open(MaterialAsset::Handle material)
{
  m_material = material;
  m_data     = material->getData();
  m_dataExtractor.extract(m_data);
  if (m_open)
    *m_open = true;

  // Reflect the freshly opened material immediately, no debounce.
  m_previewDirty    = false;
  m_timeSinceChange = 0.f;
  _rebuildPreviewMaterial();
}

void MaterialEditor::onUpdate(const DeltaTime& dt)
{
  if (m_open != nullptr && !(*m_open))
    return;
  if (!m_material.isValid())
    return;

  const float seconds = dt.seconds();

  // Spinning the preview sphere.
  m_previewSpin += seconds * 0.2f;
  if (m_previewEntity.isValid())
    m_previewEntity.getComponent<TransformComponent>().setRotation(glm::vec3(0.f, m_previewSpin, 0.f));

  // Debounced preview rebuild -> wait until edits have settled.
  if (m_previewDirty)
  {
    m_timeSinceChange += seconds;
    if (m_timeSinceChange >= s_previewDelay)
    {
      _rebuildPreviewMaterial();
      m_previewDirty = false;
    }
  }

  m_previewScene.update(dt);
}

void MaterialEditor::onRender()
{
  if (m_open != nullptr && !(*m_open))
    return;
  if (!m_material.isValid())
    return;

  m_previewScene.render();
  m_previewViewport.setTexture(m_previewScene.getRenderer().getRenderPipeline().getRenderedTexture());
}

void MaterialEditor::onImgui()
{
  if (!m_material.isValid())
    return;
  if (m_open != nullptr && !(*m_open))
    return;

  ImGuiWindowFlags flags = ImGuiWindowFlags_None;

  if (m_edited)
    flags = flags | ImGuiWindowFlags_UnsavedDocument;

  if (ImGui::Begin("Material editor", m_open, flags))
  {
    _showToolbar();
    ImGui::Separator();

    if (ImGui::BeginTable("MaterialEditorLayout", 2, ImGuiTableFlags_Resizable))
    {
      ImGui::TableNextRow();

      if (ImGui::TableNextColumn())
        _showPreview();

      if (ImGui::TableNextColumn())
      {
        bool isNode = m_material->getData().getType() != MaterialData::EType::eUndefined;

        if (ImGui::TreeNodeEx("Type name", _getTreeNodeFlags(isNode), "Material type: %s",
                              m_dataExtractor.m_typeName.c_str()))
        {
          switch (m_material->getData().getType())
          {
          case MaterialData::EType::eShadingModel:
            _showShadingModelTree();
            break;
          case MaterialData::EType::ePostProcess:
            _showPostProcessMaterialTree();
            break;
          case MaterialData::EType::eCustomShader:
            _showCustomShaderTree();
            break;
          default:
            break;
          }

          if (isNode)
            ImGui::TreePop();
        }
      }

      ImGui::EndTable();
    }
  }
  ImGui::End();
}

void MaterialEditor::_showPreview()
{
  m_previewViewport.renderImgui();
}

void MaterialEditor::_showShadingModelTree()
{
  ImGui::Indent();
  ImGui::Text("Shading model: %s", m_dataExtractor.m_shadingModelName.c_str());
  ImGui::Unindent();

  if (ImGui::TreeNodeEx("PhongParameters", _getTreeNodeFlags(true), "Parameters"))
  {
    auto params = m_data.getParameters();

    for (auto&& [name, param] : params)
    {
      ImGui::PushID(name.c_str());

      auto visitor = overloaded{
        [&, this](float& value)
        {
          return ImGui::DragFloat(param.name.c_str(), &value, 0.01f);
        },
        [&, this](glm::vec2& value)
        {
          return ImGui::DragFloat2(param.name.c_str(), &value.x, 0.01f);
        },
        [&, this](glm::vec3& value)
        {
          return ImGui::DragFloat3(param.name.c_str(), &value.x, 0.01f);
        },
        [&, this](glm::vec4& value)
        {
          return ImGui::DragFloat4(param.name.c_str(), &value.x, 0.01f);
        },
        [&, this](glm::mat4& value)
        {
          return false;
        },
        [&, this](MaterialData::TextureData& value)
        {
          std::filesystem::path path    = m_material->applyPathOrder(value.resourceName);
          TextureAsset::Handle  texture = AssetManager::Get().tryGetAsset<TextureAsset>(path);
          ImGuiInputTextFlags   flags   = 0;
          bool                  invalid = !texture.isValid();

          if (invalid)
          {
            // Notifying invalid state by a red-ish color
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.10f, 0.10f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.20f, 0.20f, 1.0f));
          }
          TextureSelector selector(texture);
          selector.setDispayName(param.name);
          selector.renderImgui();

          if (invalid)
          {
            ImGui::PopStyleColor(2);
          }

          if (selector.getChanged())
          {
            param.getValue<MaterialData::TextureData>().resourceName = selector.getAsset()->getFilePath().string();
            m_data.setParameter(param);
          }

          return selector.getChanged();
        },
      };

      if (std::visit(visitor, param.value))
      {
        m_data.setParameter(param);
        m_edited = true;
        _markPreviewDirty();
      }

      ImGui::PopID();
    }

    ImGui::TreePop();
  }
}

void MaterialEditor::_showPhongTree()
{}

void MaterialEditor::_showPBRTree()
{}

void MaterialEditor::_showCustomShaderTree()
{}

void MaterialEditor::_showPostProcessMaterialTree()
{}

void MaterialEditor::_showToolbar()
{
  ImGui::BeginDisabled(!m_edited);
  if (ImGui::Button("Save"))
    _save();
  ImGui::EndDisabled();

  ImGui::SameLine();

  // Apply: reloads the asset from disk so the changes take effect in the
  // engine. Only clickable when the material has been saved since the last
  // apply.
  ImGui::BeginDisabled(!m_savedSinceApply);
  if (ImGui::Button("Apply"))
    _apply();
  ImGui::EndDisabled();

  ImGui::SameLine();

  // Save and apply: writes to disk then reloads, in a single click.
  ImGui::BeginDisabled(!m_edited);
  if (ImGui::Button("Save and apply"))
  {
    _save();
    _apply();
  }
  ImGui::EndDisabled();
}

void MaterialEditor::_save()
{
  std::filesystem::path path = m_material->getFilePath();

  nlohmann::json j = m_data;
  if (!WriteJsonFile(path, j))
    return;

  m_edited          = false;
  m_savedSinceApply = true;
}

void MaterialEditor::_apply()
{
  // Reloading the static asset re-reads the file we just saved and
  // increments its generation, so the renderer will use the new data
  std::filesystem::path path = m_material->getFilePath();

  if (!AssetManager::Get().tryReloadAsset<MaterialAsset>(path))
  {
    VRM_LOG_ERROR("Could not apply material (reload failed): {}", path.string());
    return;
  }

  m_savedSinceApply = false;
}

ImGuiTreeNodeFlags MaterialEditor::_getTreeNodeFlags(bool isNode) const
{
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesFull;

  if (isNode)
    flags = flags | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
  else // isNode
    flags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

  return flags;
}

void MaterialEditor::PreviewViewport::onResize(const ImVec2& size)
{
  if (resizeCb)
    resizeCb(glm::uvec2(size.x, size.y));
}

void MaterialEditor::_setupPreviewScene()
{
  m_previewScene.init();

  // Sphere that displays the edited material. It keeps the .obj's default
  // material until a material is opened and the first preview build succeeds.
  m_previewEntity = m_previewScene.createEntity("PreviewSphere");
  m_previewEntity.addComponent<MeshComponent>(
    AssetManager::Get().getAsset<MeshAsset>("DefaultResources/Tests/Meshes/sphere.obj"));

  // Directional key light so shading models actually shade. Its direction is
  // the entity's forward (+x) rotated by the transform; this points it down and
  // into the scene, lighting the side that faces the camera.
  Entity light    = m_previewScene.createEntity("PreviewLight");
  auto&  dirLight = light.addComponent<DirectionalLightComponent>();
  dirLight.setIntensity(3.f);
  light.getComponent<TransformComponent>().setRotation(glm::vec3{ glm::radians(-45.f), glm::radians(45.f), 0.f });

  // Engine skybox for context.
  m_previewScene.createEntity("PreviewSkybox").addComponent<SkyboxComponent>();

  m_previewScene.setCamera(&m_previewCamera);

  {
    RenderSettings settings;
    settings.shadowsEnable     = false;
    settings.bloom.activated   = false;
    settings.antiAliasingLevel = 4;
    m_previewScene.getRenderer().getRenderPipeline().setRenderSettings(settings);
  }

  m_previewViewport.setSupportPopup(false);
  m_previewViewport.resizeCb = [this](const glm::uvec2& size)
  {
    if (size.x == 0 || size.y == 0)
      return;
    m_previewScene.onWindowResized(size);
    m_previewCamera.setAspectRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
  };
}

void MaterialEditor::_markPreviewDirty()
{
  m_previewDirty    = true;
  m_timeSinceChange = 0.f;
}

void MaterialEditor::_rebuildPreviewMaterial()
{
  if (!m_material.isValid())
    return;

  // TODO: figure out how to preview other types of material
  // (post-process ? Custom shader ? If it makes any sense)
  if (m_data.getType() != MaterialData::EType::eShadingModel)
    return;

  if (!m_previewMaterial.loadFromData(m_data, m_material->getFilePath()))
  {
    VRM_LOG_WARN("Could not build material preview from current edits.");
    return;
  }

  // Bind the preview material to the sphere once it holds a valid build.
  if (!m_previewMaterialBound && m_previewEntity.isValid())
  {
    m_previewEntity.getComponent<MeshComponent>().setMaterial(0, MaterialAsset::Handle(m_previewMaterial));
    m_previewMaterialBound = true;
  }
}
