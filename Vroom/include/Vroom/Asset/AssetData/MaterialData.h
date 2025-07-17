#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <unordered_map>

#include "Vroom/Asset/Parsing/Json.h"

namespace vrm
{

  // Represents a shading model and a bunch of parameters
  class VRM_API MaterialData
  {
  public:
    enum class EShadingModel : uint8_t
    {
      ePhong = 0,
      ePBR,
      eCount,
      eNone
    };

    struct Parameter
    {

      enum Type : uint8_t
      {
        eNone = 0,
        eFloat, eVec2, eVec3, eVec4, eMat4,
        eSampler2D
      };

      Parameter() = default;
      Parameter(const std::string& name) : name(name) {}
      Parameter& operator=(const Parameter&) = default;
      Parameter(const Parameter&) = default;
      Parameter& operator=(Parameter&&) = default;
      Parameter(Parameter&&) = default;

      template <typename T>
      inline void setValue(const T &val) { value = val; }

      template <typename T>
      inline const T& getValue() const { return std::get<T>(value); }

      std::string name;
      Type type = Type::eNone;
      std::variant<float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, std::string> value;
    };

  public:
    MaterialData();
    ~MaterialData();

    MaterialData& operator=(const MaterialData&) = default;
    MaterialData(const MaterialData&) = default;
    MaterialData& operator=(MaterialData&&) = default;
    MaterialData(MaterialData&&) = default;

    inline static const std::string &GetShadingModelName(EShadingModel model);

    void setShadingModel(EShadingModel model);
    inline EShadingModel getShadingModel() const { return m_shadingModel; }
    inline const std::string &getShadingModelName() const { return GetShadingModelName(m_shadingModel); }


    void addParameter(const Parameter &param);

    bool hasParameter(const std::string &name) const;

    const Parameter &getParameter(const std::string &name) const;

    inline const auto &getParameters() const { return m_parameters; }

    inline const auto &getTextureCount() const { return m_textureCount; }

  private:
    EShadingModel m_shadingModel = EShadingModel::eNone;
    std::unordered_map<std::string, Parameter> m_parameters;
    uint8_t m_textureCount = 0;
  };

  inline const std::string &MaterialData::GetShadingModelName(EShadingModel model)
  {
    static const std::unordered_map<EShadingModel, std::string> table = {
      { EShadingModel::ePhong, "Phong" },
      { EShadingModel::ePBR, "PBR" },
    };

    return table.at(model);
  }

} // namespace vrm

namespace nlohmann
{

  NLOHMANN_JSON_SERIALIZE_ENUM(vrm::MaterialData::EShadingModel,
  {
    { vrm::MaterialData::EShadingModel::ePhong, vrm::MaterialData::GetShadingModelName(vrm::MaterialData::EShadingModel::ePhong) },
    { vrm::MaterialData::EShadingModel::ePBR, vrm::MaterialData::GetShadingModelName(vrm::MaterialData::EShadingModel::ePBR) },
  })

  NLOHMANN_JSON_SERIALIZE_ENUM(vrm::MaterialData::Parameter::Type,
  {
    { vrm::MaterialData::Parameter::eNone, "none" },
    { vrm::MaterialData::Parameter::eFloat, "float" },
    { vrm::MaterialData::Parameter::eVec2, "vec2" },
    { vrm::MaterialData::Parameter::eVec3, "vec3" },
    { vrm::MaterialData::Parameter::eVec4, "vec4" },
    { vrm::MaterialData::Parameter::eMat4, "mat4" },
    { vrm::MaterialData::Parameter::eSampler2D, "sampler2D" },
  })

  void VRM_API to_json(json& j, const vrm::MaterialData::Parameter& e);
  void VRM_API from_json(const json& j, vrm::MaterialData::Parameter& e);

  void VRM_API to_json(json& j, const vrm::MaterialData& e);
  void VRM_API from_json(const json& j, vrm::MaterialData& e);

}