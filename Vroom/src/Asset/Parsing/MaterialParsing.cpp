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
    auto& type = j["Type"];

    if (e.isFloat())
    {
      value = e.getFloat();
      type = "float";
    }
    else if (e.isVec2())
    {
      value = e.getVec2();
      type = "vec2";
    }
    else if (e.isVec3())
    {
      value = e.getVec3();
      type = "vec3";
    }
    else if (e.isVec4())
    {
      value = e.getVec4();
      type = "vec4";
    }
    else if (e.isTexture())
    {
      value = e.getTexture();
      type = "texture";
    }
  }

  void from_json(const json& j, vrm::MaterialData::Parameter& e)
  {
    const auto& value = j["Value"];
    const auto& type = j["Type"];
    
    if (type == "float")
    {
      float v = value;
      e.setValue(v);
    }
    else if (type == "vec2")
    {
      glm::vec2 v = value;
      e.setValue(v);
    }
    else if (type == "vec3")
    {
      glm::vec3 v = value;
      e.setValue(v);
    }
    else if (type == "vec4")
    {
      glm::vec4 v = value;
      e.setValue(v);
    }
    else if (type == "texture")
    {
      std::string v = value;
      e.setValue(v);
    }
  }

  void to_json(json& j, const vrm::MaterialData& e)
  {
    j["ShadingModel"] = e.getShadingModel();
    auto& params = j["Parameters"];

    for (const auto& [name, param] : e.getParameters())
    {
      params[name] = param;
    }
  }

  void from_json(const json& j, vrm::MaterialData& e)
  {
    e = {};
    e.setShadingModel(j["ShadingModel"]);
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
