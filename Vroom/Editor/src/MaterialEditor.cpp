#define IMGUI_DEFINE_MATH_OPERATORS

#include "Editor/MaterialEditor.h"
#include <misc/cpp/imgui_stdlib.h>
#include <variant>

#include "imgui.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/JsonFile.h"
#include "AssetManager/MaterialAsset.h"
#include "AssetManager/MaterialData.h"
#include "AssetManager/TextureAsset.h"
#include "Core/DeltaTime.h"
#include "Core/Log.h"
#include "Editor/AssetSelector.h"
#include "Tools/Utility.h"

using namespace vrm;

MaterialEditor::MaterialEditor()
{}

MaterialEditor::~MaterialEditor()
{}

void MaterialEditor::open(MaterialAsset::Handle material)
{
  m_material = material;
  m_data     = material->getData();
  m_dataExtractor.extract(m_data);
  if (m_open)
    *m_open = true;
}

void MaterialEditor::onUpdate(const DeltaTime& dt)
{}

void MaterialEditor::onRender()
{}

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
  ImGui::End();
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
