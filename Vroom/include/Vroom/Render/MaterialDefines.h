#pragma once

#include <map>
#include <string>

namespace vrm
{
  
  class MaterialDefines
  {
  public:

    using Container = std::map<std::string, std::string>;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;

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
    
    inline void add(const std::string& define, const std::string& value)
    {
      add(define, std::string(value));
    }

    void add(const std::string& define, std::string&& value);

    inline const_iterator begin() const { return m_defines.begin(); }
    inline const_iterator end() const { return m_defines.end(); }

  private:

    Container m_defines;

  };

} // namespace vrm
