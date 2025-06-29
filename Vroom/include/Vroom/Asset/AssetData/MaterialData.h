#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <unordered_map>

#include "Vroom/Asset/Parsing/Json.h"

namespace vrm
{

  class VRM_API MaterialData
  {
  public:
    enum class EShadingModel : uint8_t
    {
      ePhong = 0,
      eCount,
      eDefault = ePhong
    };

    struct Parameter
    {
      Parameter() = default;
      Parameter(const std::string& name) : name(name) {}
      Parameter& operator=(const Parameter&) = default;
      Parameter(const Parameter&) = default;
      Parameter& operator=(Parameter&&) = default;
      Parameter(Parameter&&) = default;

      template <typename T>
      inline void setValue(const T &val) { value = val; }

      inline bool isFloat() const { return std::holds_alternative<float>(value); }
      inline const float &getFloat() const { return std::get<float>(value); }

      inline bool isVec2() const { return std::holds_alternative<glm::vec2>(value); }
      inline const glm::vec2 &getVec2() const { return std::get<glm::vec2>(value); }

      inline bool isVec3() const { return std::holds_alternative<glm::vec3>(value); }
      inline const glm::vec3 &getVec3() const { return std::get<glm::vec3>(value); }

      inline bool isVec4() const { return std::holds_alternative<glm::vec4>(value); }
      inline const glm::vec4 &getVec4() const { return std::get<glm::vec4>(value); }

      inline bool isTexture() const { return std::holds_alternative<std::string>(value); }
      inline const std::string &getTexture() const { return std::get<std::string>(value); }

      std::string name;
      std::variant<float, glm::vec2, glm::vec3, glm::vec4, std::string> value;
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
    EShadingModel m_shadingModel = EShadingModel::eDefault;
    std::unordered_map<std::string, Parameter> m_parameters;
    uint8_t m_textureCount = 0;
  };

  inline const std::string &MaterialData::GetShadingModelName(EShadingModel model)
  {
    static const std::unordered_map<EShadingModel, std::string> table = {
      { EShadingModel::ePhong, "Phong" }
    };

    return table.at(model);
  }

} // namespace vrm

namespace nlohmann
{

  NLOHMANN_JSON_SERIALIZE_ENUM(vrm::MaterialData::EShadingModel,
  {
    { vrm::MaterialData::EShadingModel::ePhong, vrm::MaterialData::GetShadingModelName(vrm::MaterialData::EShadingModel::ePhong) }
  })

  void VRM_API to_json(json& j, const vrm::MaterialData::Parameter& e);
  void VRM_API from_json(const json& j, vrm::MaterialData::Parameter& e);

  void VRM_API to_json(json& j, const vrm::MaterialData& e);
  void VRM_API from_json(const json& j, vrm::MaterialData& e);

}