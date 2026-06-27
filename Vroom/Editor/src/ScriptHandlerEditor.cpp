#include "Editor/ScriptHandlerEditor.h"

#include <AssetManager/AssetManager.h>
#include <AssetManager/ScriptAsset.h>

#include "Editor/AssetSelector.h"
#include "Editor/EditorLayer.h"

using namespace vrm;

void ScriptHandlerEditor::editEntityComponent(Entity& e) const
{
  auto& component = get(e);

  const bool          hasScript       = component.hasScript();
  const std::string   currentScriptId = hasScript ? component.getScript().getScriptName() : "";
  ScriptAsset::Handle selectedScript{};
  if (hasScript)
  {
    selectedScript = AssetManager::Get().tryGetAsset<ScriptAsset>(currentScriptId);
  }

  ScriptSelector selector(selectedScript);
  selector.renderImgui();

  if (selector.getChanged())
  {
    auto selectedScriptAsset = selector.getAsset();
    if (selectedScriptAsset.isValid())
    {
      const std::string requestedScriptId = selectedScriptAsset->getScriptId();
      if (requestedScriptId != currentScriptId)
      {
        Entity targetEntity = e;
        EditorLayer::Get().pushFrameEndRoutine(
          [targetEntity, requestedScriptId](auto&) mutable
          {
            if (!targetEntity.isValid() || !targetEntity.hasComponent<ScriptHandler>())
              return;

            targetEntity.removeComponent<ScriptHandler>();
            targetEntity.addScriptComponent(requestedScriptId);
          });
      }
    }
  }
}
