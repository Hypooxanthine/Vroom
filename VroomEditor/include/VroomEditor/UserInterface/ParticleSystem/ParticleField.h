#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include <array>
#include <span>
#include <string>

namespace vrm
{

  class ParticleField : public ImGuiElement
  {
  public:

    enum class EType
    {
      eScalar = 0, eVec2, eVec3, eVec4,
      eColor3, eColor4,
      eCount
    };

  public:

    ParticleField();
    ~ParticleField();

    void setName(const std::string& name);
    void setType(EType type);
    void setBounds(std::span<float const> min, std::span<float const> max);
    void setLockScale(bool lock);
    
    inline EType getType() const { return m_type; }
    inline int getDimension() const { return m_dim; }
    inline bool isScaleLocked() const { return m_scaleLock; }
    inline std::span<float const> getValue() const { return std::span(data.data(), m_dim); }
    inline std::span<float const> getMin() const { return std::span(m_min.data(), m_dim); }
    inline std::span<float const> getMax() const { return std::span(m_max.data(), m_dim); }

  protected:

    void onImgui() override;
    virtual void onImguiEdit() = 0;
    inline bool isColorType() const { return m_type == EType::eColor3 || m_type == EType::eColor4; }

  protected:

    std::array<float, 4> data = { 0.f, 0.f, 0.f, 0.f };

  private:

    std::string m_name = "Field";
    EType m_type = EType::eScalar;
    int m_dim = 1;
    bool m_scaleLock = false;
    std::array<float, 4> m_min = { 0.f, 0.f, 0.f, 0.f};
    std::array<float, 4> m_max = { 1.f, 1.f, 1.f, 1.f};

  };

  class ConstParticleField : public ParticleField
  {
  public:

  protected:

    void onImguiEdit() override;

  private:

  };

}