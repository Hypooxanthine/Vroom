#pragma once

#include <string>
#include <vector>

#include "VroomEditor/UserInterface/AssetSelector.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"

namespace vrm::editor
{

class EmitterEditor : public ImGuiElement
{
public:

  EmitterEditor();
  ~EmitterEditor();

  EmitterEditor& operator=(const EmitterEditor& other) = delete;
  EmitterEditor(const EmitterEditor& other)            = delete;

  EmitterEditor& operator=(EmitterEditor&& other) = default;
  EmitterEditor(EmitterEditor&& other)            = default;

  bool requestedDelete() const { return m_requestDelete; }

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;
  void setName(const std::string& name);

protected:

  void onImgui() override;
  void onUpdate(const DeltaTime& dt) override;

private:

  void _showContextualMenu();

private:

  mutable bool                                         m_changed       = true;
  bool                                                 m_requestDelete = false;
  float                                                m_emitRate      = 1.f;
  float                                                m_lifeTime      = 2.f;
  std::string                                          m_name = "Attribute";
  MeshSelector                                         m_meshSelector;
  std::vector<std::unique_ptr<EmitterAttributeEditor>> m_attributes;
};

} // namespace vrm::editor
