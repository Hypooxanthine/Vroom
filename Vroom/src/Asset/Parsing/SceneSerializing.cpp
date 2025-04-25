#include "Vroom/Asset/Parsing/SceneParsing.h"

using namespace vrm;

#include "Vroom/Asset/Parsing/Json.h"

void SceneParsing::Serialize(const SceneData& sceneData, json& j)
{
  to_json(j, sceneData);
}
