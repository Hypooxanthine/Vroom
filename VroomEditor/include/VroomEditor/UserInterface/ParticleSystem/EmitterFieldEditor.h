#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{
class IEmitterField;
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
      eConst = 0,
      eRandomRange,
      eRandomCone,
      eCount
    };
  };

  [[nodiscard]] inline static EmitterFieldEditor*
  Instanciate(EType::Type fieldType);

public:

  EmitterFieldEditor(EType::Type type);
  ~EmitterFieldEditor();

  bool updateEmitterField(std::unique_ptr<IEmitterField>& field) const;

  void                      setName(const std::string& name);
  inline const std::string& getName() const { return m_name; }

  inline bool requestedNewFieldType() const { return m_type != m_nextType; }
  inline EType::Type getNextType() const { return m_nextType; }

  inline void setSupportedTypes(const std::unordered_set<EType::Type>& types)
  {
    m_supportedTypes = types;
  }

  inline void setScalarSettings(const EmitterScalarEditor::Settings& settings)
  {
    m_scalarSettings = settings;
    onUpdateScalarSettings(settings);
  }

  inline const EmitterScalarEditor::Settings& getScalarSettings() const
  {
    return m_scalarSettings;
  }

protected:

  void         onImgui() override;
  virtual void onImguiEdit() = 0;
  virtual bool
  onUpdateEmitterField(std::unique_ptr<IEmitterField>& field) const = 0;
  virtual void
  onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) = 0;

private:

  void _showContextualMenu();

private:

  std::string                     m_name = "Field";
  std::unordered_set<EType::Type> m_supportedTypes;
  EmitterScalarEditor::Settings   m_scalarSettings;
  EType::Type                     m_type;
  EType::Type                     m_nextType;
};

class ConstEmitterFieldEditor : public EmitterFieldEditor
{
public:

  ConstEmitterFieldEditor();
  ~ConstEmitterFieldEditor();

protected:

  void onImguiEdit() override;
  bool
  onUpdateEmitterField(std::unique_ptr<IEmitterField>& field) const override;
  inline void
  onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) override
  {
    m_scalarEditor.setSettings(settings);
  }

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
  bool
  onUpdateEmitterField(std::unique_ptr<IEmitterField>& field) const override;

  inline void
  onUpdateScalarSettings(const EmitterScalarEditor::Settings& settings) override
  {
    m_minRange.setSettings(settings);
    m_maxRange.setSettings(settings);
  }

private:

  EmitterScalarEditor m_minRange, m_maxRange;
};

class RandomConeEmitterFieldEditor : public EmitterFieldEditor
{
public:

  RandomConeEmitterFieldEditor();
  ~RandomConeEmitterFieldEditor();

protected:

  void onImguiEdit() override;
  bool
  onUpdateEmitterField(std::unique_ptr<IEmitterField>& field) const override;

  void onUpdateScalarSettings(
    const EmitterScalarEditor::Settings& settings) override;

private:

  EmitterScalarEditor m_direction;
  EmitterScalarEditor m_angle;
  EmitterScalarEditor m_length;
};

inline EmitterFieldEditor*
EmitterFieldEditor::Instanciate(EType::Type fieldType)
{
  switch (fieldType)
  {
  case vrm::editor::EmitterFieldEditor::EType::eConst:
    return new ConstEmitterFieldEditor();
    break;
  case vrm::editor::EmitterFieldEditor::EType::eRandomRange:
    return new RandomRangeEmitterFieldEditor();
    break;
  case vrm::editor::EmitterFieldEditor::EType::eRandomCone:
    return new RandomConeEmitterFieldEditor();
    break;
  default:
    VRM_ASSERT_MSG(false, "Unsupported field type");
    break;
  }
}

} // namespace vrm::editor

namespace std
{
inline std::string
to_string(vrm::editor::EmitterFieldEditor::EType::Type fieldType)
{
  switch (fieldType)
  {
  case vrm::editor::EmitterFieldEditor::EType::eConst:
    return "Constant";
    break;
  case vrm::editor::EmitterFieldEditor::EType::eRandomRange:
    return "Random range";
    break;
  case vrm::editor::EmitterFieldEditor::EType::eRandomCone:
    return "Random cone";
    break;
  default:
    return "Undefined";
    break;
  }
}
} // namespace std
