#pragma once

#include <map>
#include <string>

namespace vrm
{
  
  class MaterialDefines
  {
  public:

    MaterialDefines();
    ~MaterialDefines();

    MaterialDefines& operator=(const MaterialDefines& other) = delete;
    MaterialDefines(const MaterialDefines& other) = delete;

    MaterialDefines& operator=(MaterialDefines&& other) = delete;
    MaterialDefines(MaterialDefines&& other) = delete;

    template <typename T>
    void add(const std::string& define, T&& value)
    {
      std::string strValue = std::to_string(std::forward<T>(value));
      add(define, std::move(strValue));
    }

    void add(const std::string& define, std::string&& value);

  private:

    std::map<std::string, std::string> m_defines;

  };

} // namespace vrm
