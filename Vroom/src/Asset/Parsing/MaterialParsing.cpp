#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include <unordered_map>

#include <glm/gtx/string_cast.hpp>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/Json.h"

using namespace vrm;

bool MaterialParsing::Parse(const json &jsonData, MaterialData &outMaterialData)
{
  outMaterialData = jsonData;

  return true;
}


namespace nlohmann
{

  void to_json(json& j, const vrm::MaterialData::Parameter& e)
  {
    auto& value = j["Value"];
    j["Type"] = e.type;

    switch(e.type)
    {
      case vrm::MaterialData::Parameter::eFloat:
      {
        value = e.getValue<float>();
        break;
      }
      case vrm::MaterialData::Parameter::eVec2:
      {
        value = e.getValue<glm::vec2>();
        break;
      }
      case vrm::MaterialData::Parameter::eVec3:
      {
        value = e.getValue<glm::vec3>();
        break;
      }
      case vrm::MaterialData::Parameter::eVec4:
      {
        value = e.getValue<glm::vec4>();
        break;
      }
      case vrm::MaterialData::Parameter::eMat4:
      {
        value = e.getValue<glm::mat4>();
        break;
      }
      case vrm::MaterialData::Parameter::eSampler2D:
      {
        value = e.getValue<std::string>();
        break;
      }
      default:
      {
        VRM_ASSERT_MSG(false, "Unsupported type");
        break;
      }
    }
  }

  void from_json(const json& j, vrm::MaterialData::Parameter& e)
  {
    const auto& value = j["Value"];
    e.type = j["Type"];

    switch(e.type)
    {
      case vrm::MaterialData::Parameter::eFloat:
      {
        e.setValue<float>(value);
        break;
      }
      case vrm::MaterialData::Parameter::eVec2:
      {
        e.setValue<glm::vec2>(value);
        break;
      }
      case vrm::MaterialData::Parameter::eVec3:
      {
        e.setValue<glm::vec3>(value);
        break;
      }
      case vrm::MaterialData::Parameter::eVec4:
      {
        e.setValue<glm::vec4>(value);
        break;
      }
      case vrm::MaterialData::Parameter::eMat4:
      {
        e.setValue<glm::mat4>(value);
        break;
      }
      case vrm::MaterialData::Parameter::eSampler2D:
      {
        e.setValue<std::string>(value);
        break;
      }
      default:
      {
        VRM_ASSERT_MSG(false, "Unsupported type");
        break;
      }
    }
  }

  void to_json(json& j, const vrm::MaterialData& e)
  {
    j["Type"] = e.getType();

    if (e.getShadingModel() != MaterialData::EShadingModel::eNone)
      j["ShadingModel"] = e.getShadingModel();
    
    if (e.getCustomShader().size() > 0)
      j["CustomShader"] = e.getCustomShader();
    
    auto& params = j["Parameters"];

    for (const auto& [name, param] : e.getParameters())
    {
      params[name] = param;
    }
  }

  void from_json(const json& j, vrm::MaterialData& e)
  {
    e = {};

    vrm::MaterialData::EType type = j.at("Type");
    e.setType(type);

    if (j.contains("ShadingModel"))
      e.setShadingModel(j["ShadingModel"]);

    if (j.contains("CustomShader"))
      e.setCustomShader(j["CustomShader"]);

    if (!j.contains("Parameters"))
      return;
    
    const json& params = j["Parameters"];

    if (params.is_null())
    {
      return;
    }

    for (const auto& [name, param] : params.get<json::object_t>())
    {
      vrm::MaterialData::Parameter p;
      p = param;
      p.name = name;
      e.addParameter(p);
    }
  }

}
