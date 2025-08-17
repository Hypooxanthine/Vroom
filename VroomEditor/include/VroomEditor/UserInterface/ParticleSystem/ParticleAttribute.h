#pragma once

#include "Vroom/Render/ParticleEmitter.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/ParticleField.h"

namespace vrm
{

  class ParticleAttribute : public ImGuiElement
  {
  public:

    struct Specs
    {
      ParticleField::EType type = ParticleField::EType::eScalar;
      float min = -10.f, max = 10.f;
      bool scaleLocked = false;
    };

  public:

    ParticleAttribute(const std::string& name = "Attribute");
    ~ParticleAttribute();

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;

    void setName(const std::string& name);
    void setEmitterSpecsOffset(size_t offset);
    void setSpecs(const Specs& specs);
  
  protected:

    void onImgui() override;

  private:

    std::string m_name;

    ConstParticleField m_spawnValue;
    ConstParticleField m_deathValue;

  };



}