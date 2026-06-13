#define IMGUI_DEFINE_MATH_OPERATORS

#include "Editor/MaterialEditor.h"
#include <misc/cpp/imgui_stdlib.h>
#include <variant>

#include "imgui.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/MaterialAsset.h"
#include "AssetManager/MaterialData.h"
#include "AssetManager/TextureAsset.h"
#include "Core/DeltaTime.h"
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

  constexpr auto flags = ImGuiWindowFlags_None;

  if (ImGui::Begin("Material editor", m_open, flags))
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
          return ImGui::InputFloat(param.name.c_str(), &value);
        },
        [&, this](glm::vec2& value)
        {
          return ImGui::InputFloat2(param.name.c_str(), &value.x);
        },
        [&, this](glm::vec3& value)
        {
          return ImGui::InputFloat3(param.name.c_str(), &value.x);
        },
        [&, this](glm::vec4& value)
        {
          return ImGui::InputFloat4(param.name.c_str(), &value.x);
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

ImGuiTreeNodeFlags MaterialEditor::_getTreeNodeFlags(bool isNode) const
{
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesFull;

  if (isNode)
    flags = flags | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
  else // isNode
    flags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

  return flags;
}
