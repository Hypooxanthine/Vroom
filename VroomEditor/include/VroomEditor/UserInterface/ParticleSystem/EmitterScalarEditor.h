#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include <cstddef>

namespace vrm::editor
{

  class EmitterScalarEditor : public ImGuiElement
  {
  public:

    enum class EScalarType
    {
      eScalar = 0, eVec2, eVec3, eVec4,
      eColor3, eColor4,
      eCount
    };

    struct Settings
    {
      EScalarType scalarType = EScalarType::eScalar;
      std::vector<float> minValue, maxValue;
      bool scaleLocked = false;
    };

  public:

    inline bool getModified() const { return m_dataModified; }

    void setSettings(const Settings& settings);
    void setScalarType(EScalarType type);
    void setBounds(std::span<float const> min, std::span<float const> max);
    void setLockScale(bool lock);
    void setData(std::span<float const> data);

    inline EScalarType getScalarType() const { return m_type; }
    inline size_t getDimension() const { return m_dim; }
    inline bool isScaleLocked() const { return m_scaleLock; }
    inline std::span<float const> getMin() const { return std::span(m_min.data(), m_dim); }
    inline std::span<float const> getMax() const { return std::span(m_max.data(), m_dim); }
    inline bool isColorType() const { return m_type == EScalarType::eColor3 || m_type == EScalarType::eColor4; }
    std::span<float const> getData() const;

  protected:

    void onImgui() override;

    inline void markDataModified() { m_dataModified = true; }

  private:

    EScalarType m_type = EScalarType::eScalar;
    bool m_dataModified = false;
    size_t m_dim = 1;
    bool m_scaleLock = false;
    std::array<float, 4> m_min = { 0.f, 0.f, 0.f, 0.f};
    std::array<float, 4> m_max = { 1.f, 1.f, 1.f, 1.f};
    std::array<float, 4> m_data = { 0.f, 0.f, 0.f, 0.f };
  };

}