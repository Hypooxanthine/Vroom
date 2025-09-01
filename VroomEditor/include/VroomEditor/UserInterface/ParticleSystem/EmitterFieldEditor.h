#pragma once

#include "Vroom/Core/Assert.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"
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

    struct EType
    {
      enum Type : size_t
      {
        eConstEmitterField = 0, eRandomRangeEmitterField,
        eCount
      };
    };

    inline static EmitterFieldEditor* Instanciate(EType::Type fieldType);

  public:

    EmitterFieldEditor(EType::Type type);
    ~EmitterFieldEditor();

    bool updateEmitterField(ParticleEmitterFieldBase& field) const;

    void setName(const std::string& name);
    inline const std::string& getName() const { return m_name; }

    inline bool requestedNewFieldType() const { return m_type != m_nextType; }
    inline EType::Type getNextType() const { return m_nextType; }

    inline void setScalarSettings(const EmitterScalarEditor::Settings& settings) { onUpdateScalarSettings(settings); }

  protected:

    void onImgui() override;
    virtual void onImguiEdit() = 0;
    virtual bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const = 0;
    virtual void onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) = 0;

  private:

    void _showContextualMenu();

  private:

    std::string m_name = "Field";
    EType::Type m_type;
    EType::Type m_nextType;

  };

  class ConstEmitterFieldEditor : public EmitterFieldEditor
  {
  public:

    ConstEmitterFieldEditor();
    ~ConstEmitterFieldEditor();

  protected:

    void onImguiEdit() override;
    bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const override;
    inline void onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) override { m_scalarEditor.setSettings(settings); }

  private:

    EmitterScalarEditor m_scalarEditor;
  };

  class RandomRangeEmitterFieldEditor : public EmitterFieldEditor
  {
  public:

    RandomRangeEmitterFieldEditor();
    ~RandomRangeEmitterFieldEditor();

  protected:

    void onImguiEdit() override;
    bool onUpdateEmitterField(ParticleEmitterFieldBase& field) const override;

    inline void onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) override
    {
      m_minRange.setSettings(settings);
      m_maxRange.setSettings(settings);
    }

  private:
  
    EmitterScalarEditor m_minRange, m_maxRange;

  };

  inline EmitterFieldEditor* EmitterFieldEditor::Instanciate(EType::Type fieldType)
  {
    switch (fieldType)
    {
    case vrm::editor::EmitterFieldEditor::EType::eConstEmitterField:
      return new ConstEmitterFieldEditor();
      break;
    case vrm::editor::EmitterFieldEditor::EType::eRandomRangeEmitterField:
      return new RandomRangeEmitterFieldEditor();
      break;
    default:
      VRM_ASSERT_MSG(false, "Unsupported field type");
      break;
    }
  }

}

namespace std
{
  inline std::string to_string(vrm::editor::EmitterFieldEditor::EType::Type fieldType)
  {
    switch (fieldType)
    {
    case vrm::editor::EmitterFieldEditor::EType::eConstEmitterField:
      return "Constant";
      break;
    case vrm::editor::EmitterFieldEditor::EType::eRandomRangeEmitterField:
      return "Random range";
      break;
    default:
      return "Undefined";
      break;
    }
  }
}
