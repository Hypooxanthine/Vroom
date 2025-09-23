#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

#include "imgui.h"

#include "Vroom/Core/Assert.h"

using namespace vrm::editor;

void EmitterScalarEditor::onImgui()
{
  static constexpr std::string_view widgetLabel = "##";

  if (isColorType())
  {
    ImGuiColorEditFlags flags =
      0 | (getDimension() == 3 ? ImGuiColorEditFlags_NoAlpha : 0);
    if (ImGui::ColorEdit4(widgetLabel.data(), m_data.data(), flags))
    {
      markDataModified();
    }
  }
  else
  {
    ImGuiSliderFlags flags = 0 | ImGuiSliderFlags_ClampOnInput;

    if (ImGui::SliderScalarN(widgetLabel.data(), ImGuiDataType_Float,
                             m_data.data(), getDimension(), getMin().data(),
                             getMax().data(), nullptr, flags))
    {
      markDataModified();
    }
  }
}

void EmitterScalarEditor::setSettings(const Settings& settings)
{
  setScalarType(settings.scalarType);
  setBounds(settings.minValue, settings.maxValue);
  setLockScale(settings.scaleLocked);
  setData(settings.defaultValue);
}

void EmitterScalarEditor::setBounds(std::span<float const> min,
                                    std::span<float const> max)
{
  VRM_ASSERT(min.size() == max.size() && min.size() == getDimension());

  for (int i = 0; i < getDimension(); ++i)
  {
    m_min[i] = min[i];
    m_max[i] = max[i];
  }
}

void EmitterScalarEditor::setScalarType(EScalarType type)
{
  switch (type)
  {
  case EScalarType::eScalar:
    m_dim = 1;
    break;
  case EScalarType::eVec2:
    m_dim = 2;
    break;
  case EScalarType::eVec3:
  case EScalarType::eColor3:
    m_dim = 3;
    break;
  case EScalarType::eVec4:
  case EScalarType::eColor4:
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

void EmitterScalarEditor::setLockScale(bool lock) { m_scaleLock = lock; }

void EmitterScalarEditor::setData(std::span<float const> data)
{
  VRM_ASSERT_MSG(data.size() == getDimension(),
                 "Field dimension must match value size");
  for (size_t i = 0; i < getDimension(); ++i) { m_data[i] = data[i]; }
}

std::span<float const> EmitterScalarEditor::getData() const
{
  return std::span{ &m_data[0], m_dim };
}
