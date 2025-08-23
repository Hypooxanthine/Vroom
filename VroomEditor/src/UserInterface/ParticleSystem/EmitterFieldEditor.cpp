#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "Vroom/Core/Assert.h"
#include "imgui.h"
#include <string_view>

using namespace vrm::editor;

EmitterFieldEditor::EmitterFieldEditor()
{

}

EmitterFieldEditor::~EmitterFieldEditor()
{

}

void EmitterFieldEditor::setName(const std::string& name)
{
  m_name = name;
}

void EmitterFieldEditor::setType(EType type)
{
  switch(type)
  {
  case EType::eScalar:
    m_dim = 1;
    break;
  case EType::eVec2:
    m_dim = 2;
    break;
  case EType::eVec3:
  case EType::eColor3:
    m_dim = 3;
    break;
  case EType::eVec4:
  case EType::eColor4:
    m_dim = 4;
    break;
  default:
    VRM_ASSERT_MSG(false, "Unsupported particle field type");
    return;
  }

  m_type = type;

  if (isColorType())
  {
    for (size_t i = 0; i < 4; ++i)
    {
      m_min[i] = std::max(0.f, m_min[i]);
      m_max[i] = std::min(0.f, m_max[i]);
    }
  }
}

void EmitterFieldEditor::setLockScale(bool lock)
{
  m_scaleLock = lock;
}

void EmitterFieldEditor::setValue(std::span<float const> value)
{
  VRM_ASSERT_MSG(value.size() == m_dim, "Field dimension must match value size");
  for (size_t i = 0; i < m_dim; ++i)
  {
    data[i] = value[i];
  }
}

void EmitterFieldEditor::setBounds(std::span<float const> min, std::span<float const> max)
{
  VRM_ASSERT(min.size() == max.size() && min.size() == getDimension());

  for (int i = 0; i < getDimension(); ++i)
  {
    m_min[i] = min[i];
    m_max[i] = max[i];
  }
}

void EmitterFieldEditor::onImgui()
{
  m_dataModified = false;

  ImGui::Text("%s:", m_name.c_str());
  ImGui::SameLine();
  onImguiEdit();
}

bool EmitterFieldEditor::updateEmitterField(ParticleEmitterFieldBase& field) const
{
  if (m_dataModified)
  {
    onUpdateEmitterField(field);
    return true;
  }
  else
  {
    return false;
  }
}

void ConstParticleField::onImguiEdit()
{
  static constexpr std::string_view widgetLabel = "##";

  if (isColorType())
  {
    ImGuiColorEditFlags flags = 0
      | (getDimension() == 3 ? ImGuiColorEditFlags_NoAlpha : 0)
    ;
    if (ImGui::ColorEdit4(widgetLabel.data(), data.data(), flags))
    {
      markDataModified();
    }
  }
  else
  {
    ImGuiSliderFlags flags = 0
      | ImGuiSliderFlags_ClampOnInput
    ;

    if (ImGui::SliderScalarN(
          widgetLabel.data(),
          ImGuiDataType_Float,
          data.data(),
          getDimension(),
          getMin().data(),
          getMax().data(),
          nullptr,
          flags)
    )
    {
      markDataModified();
    }
  }
}

void ConstParticleField::onUpdateEmitterField(ParticleEmitterFieldBase& field) const
{
  std::span<float> emitterFieldData = field.getRawData();
  VRM_ASSERT_MSG(emitterFieldData.size() <= data.size(), "Unexpected error: emitter field size is too big");

  for (size_t i = 0; i < emitterFieldData.size(); ++i)
  {
    emitterFieldData[i] = data[i];
  }
}
