#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"
#include <array>
#include <string>

namespace vrm
{
  struct ParticleEmitterFieldBase;
}

namespace vrm::editor
{

  class EmitterFieldEditor : public ImGuiElement
  {
  public:

    enum class EType
    {
      eConstEmitterField = 0, eRandomRangeEmitterField,
      eCount
    };

  public:

    EmitterFieldEditor();
    ~EmitterFieldEditor();

    void setData(const ParticleEmitterFieldBase& field);
    bool updateEmitterField(ParticleEmitterFieldBase& field) const;

    void setName(const std::string& name);

  protected:

    void onImgui() override;
    virtual void onImguiEdit() = 0;
    virtual void onSetData(const ParticleEmitterFieldBase& field) = 0;
    virtual bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const = 0;

  private:

    std::string m_name = "Field";

  };

  class ConstEmitterFieldEditor : public EmitterFieldEditor
  {
  public:

    inline void setScalarSettings(const EmitterScalarEditor::Settings& settings) { m_scalarEditor.setSettings(settings); }

  protected:

    void onImguiEdit() override;
    void onSetData(const ParticleEmitterFieldBase& field) override;
    bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const override;

  private:

    EmitterScalarEditor m_scalarEditor;
  };

  class RandomRangeEmitterFieldEditor : public EmitterFieldEditor
  {
  public:

  protected:

    void onImguiEdit() override;
    void onSetData(const ParticleEmitterFieldBase& field) override;
    bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const override;

  private:

    std::array<float, 4>
      dataMin = { 0.f, 0.f, 0.f, 0.f },
      dataMax = { 0.f, 0.f, 0.f, 0.f }
    ;

  };

}