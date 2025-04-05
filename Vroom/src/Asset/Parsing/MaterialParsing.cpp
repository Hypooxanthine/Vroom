#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include <unordered_map>

#include <glm/gtx/string_cast.hpp>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/Json.h"

using namespace vrm;

static bool ParseParameter(const json& j, MaterialData::Parameter& param)
{
  CHECK_ATTR_STRING(j, Type);
  CHECK_ATTR(j, Value);

  if (TypeVal == "float")
  {
    float val;
    CHECK(GetParamValue(Value, val), "Could not parse parameter value {} as a {}", val, TypeVal);
    param.value = val;
  }
  else if (TypeVal == "vec2")
  {
    glm::vec2 val;
    CHECK(GetParamValue(Value, val), "Could not parse parameter value {} as a {}", glm::to_string(val), TypeVal);
    param.value = val;
  }
  else if (TypeVal == "vec3")
  {
    glm::vec3 val;
    CHECK(GetParamValue(Value, val), "Could not parse parameter value {} as a {}", glm::to_string(val), TypeVal);
    param.value = val;
  }
  else if (TypeVal == "vec4")
  {
    glm::vec4 val;
    CHECK(GetParamValue(Value, val), "Could not parse parameter value {} as a {}", glm::to_string(val), TypeVal);
    param.value = val;
  }
  else if (TypeVal == "texture")
  {
    std::string val;
    CHECK(GetParamValue(Value, val), "Could not parse parameter value {} as a {}", val, TypeVal);
    param.value = val;
  }
  else
  {
    CHECK(false, "Unknown parameter type: {}", TypeVal);
  }

  return true;
}

static bool ParseParameters(const json& j, MaterialData& mat)
{
  CHECK_ATTR_OBJECT(j, Parameters);

  for (const auto& [j_name, j_object] : ParametersVal)
  {
    MaterialData::Parameter param;
    param.name = j_name;
    CHECK(ParseParameter(j_object, param), "Error while parsing parameter");
    mat.addParameter(param);
  }

  return true;
}

static bool ParseShadingModelName(const json &j, MaterialData &mat)
{
  CHECK_ATTR_STRING(j, ShadingModel);

  for (uint8_t i = 0; i < static_cast<uint8_t>(MaterialData::EShadingModel::eCount); ++i)
  {
    auto modelEnum = static_cast<MaterialData::EShadingModel>(i);
    if (MaterialData::GetShadingModelName(modelEnum) == ShadingModelVal)
    {
      mat.setShadingModel(modelEnum);
      return true;
    }
  }

  CHECK(false, "Unknown ShadingModel value: {}", ShadingModelVal);
}

bool MaterialParsing::Parse(const json &jsonData, MaterialData &outSceneData)
{
  CHECK_OBJECT(jsonData);

  MaterialData mat;

  CHECK(ParseShadingModelName(jsonData, mat), "Error while parsing shading model name");
  CHECK(ParseParameters(jsonData, mat), "Error while parsing ShadingModel parameters");

  outSceneData = std::move(mat);

  return true;
}
