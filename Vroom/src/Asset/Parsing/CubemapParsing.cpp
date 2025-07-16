#include "Vroom/Asset/Parsing/CubemapParsing.h"

using namespace vrm;

namespace nlohmann
{

  void to_json(json& j, const CubemapData::JsonLayout& cubemap)
  {
    j = json::object();
    j["posx"] = cubemap.posx;
    j["negx"] = cubemap.negx;

    j["posy"] = cubemap.posy;
    j["negy"] = cubemap.negy;

    j["posz"] = cubemap.posz;
    j["negz"] = cubemap.negz;
  }
  
  void from_json(const json& j, CubemapData::JsonLayout& cubemap)
  {
    VRM_ASSERT_MSG(j.is_object(), "Cubemap json data must be an object");
    
    cubemap.posx = j.at("posx");
    cubemap.negx = j.at("negx");

    cubemap.posy = j.at("posy");
    cubemap.negy = j.at("negy");

    cubemap.posz = j.at("posz");
    cubemap.negz = j.at("negz");
  }

} // namespace nlohmann
