#include "VroomEditor/UserInterface/ParticleSystem/ParticleField.h"
#include "Vroom/Core/Assert.h"
#include "imgui.h"
#include <string_view>

using namespace vrm;

ParticleField::ParticleField()
{

}

ParticleField::~ParticleField()
{

}

void ParticleField::setName(const std::string& name)
{
  m_name = name;
}

void ParticleField::setType(EType type)
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

void ParticleField::setLockScale(bool lock)
{
  m_scaleLock = lock;
}

void ParticleField::setBounds(std::span<float const> min, std::span<float const> max)
{
  VRM_ASSERT(min.size() == max.size() && min.size() == getDimension());

  for (int i = 0; i < getDimension(); ++i)
  {
    m_min[i] = min[i];
    m_max[i] = max[i];
  }
}

void ParticleField::onImgui()
{
  ImGui::Text("%s:", m_name.c_str());
  ImGui::SameLine();
  onImguiEdit();
}

void ConstParticleField::onImguiEdit()
{
  std::array<float, 4> localData = data;
  static constexpr std::string_view widgetLabel = "##";

  if (isColorType())
  {
    ImGuiColorEditFlags flags = 0
      | (getDimension() == 3 ? ImGuiColorEditFlags_NoAlpha : 0)
    ;
    ImGui::ColorEdit4(widgetLabel.data(), localData.data(), flags);
  }
  else
  {
    ImGuiSliderFlags flags = 0
      | ImGuiSliderFlags_ClampOnInput
    ;

    ImGui::SliderScalarN(
      widgetLabel.data(),
      ImGuiDataType_Float,
      localData.data(),
      getDimension(),
      getMin().data(),
      getMax().data(),
      nullptr,
      flags
    );
  }

  // const char* label = "";
  // void* v = localData.data();
  // int components = getDimension();
  // auto data_type = ImGuiDataType_Float;
  // const char* format = nullptr;
  // const void* v_min = getMin().data();
  // const void* v_max = getMax().data();
  // ImGuiSliderFlags flags = 0;

  // ImGuiWindow* window = ImGui::GetCurrentWindow();
  // if (window->SkipItems)
  //     return;

  // ImGuiContext& g = *GImGui;
  // bool value_changed = false;
  // ImGui::BeginGroup();
  // ImGui::PushID(label);
  // ImGui::PushMultiItemsWidths(components, ImGui::CalcItemWidth());
  // size_t type_size = sizeof(float);
  // for (int i = 0; i < components; i++)
  // {
  //   ImGui::PushID(i);
  //   if (i > 0)
  //     ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
  //   value_changed |= ImGui::Sliderdata("", data_type, v, v_min, v_max, format, flags);
  //   ImGui::PopID();
  //   ImGui::PopItemWidth();
  //   v = (void*)((char*)v + type_size);
  // }
  // ImGui::PopID();

  // const char* label_end = ImGui::FindRenderedTextEnd(label);
  // if (label != label_end)
  // {
  //   ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
  //   ImGui::TextEx(label, label_end);
  // }

  // ImGui::EndGroup();
}
