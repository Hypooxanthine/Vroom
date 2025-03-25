#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace vrm
{

class ShaderParsing
{
public:
  struct Results
  {
    std::string computeSource;
    std::string vertexSource;
    std::string geometrySource;
    std::string fragmentSource;
  };
public:
  ShaderParsing() = delete;

  static Results Parse(const nlohmann::json& jsonData);

private:

};

} // namespace vrm
