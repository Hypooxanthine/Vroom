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
    
    struct Hasher
    {
      inline size_t operator()(const MaterialDefines& defines) const
      {
        return defines.getHash();
      }
    };

    friend struct Equals;
    struct Equals
    {
      inline bool operator()(const MaterialDefines& lhs, const MaterialDefines& rhs) const
      {
        return lhs.m_defines == rhs.m_defines;
      }
    };

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
      m_hashDirty = true;
    }

    void add(const std::string& define, std::string&& value);

    inline const_iterator begin() const { return m_defines.begin(); }
    inline const_iterator end() const { return m_defines.end(); }

    inline size_t getHash() const
    {
      if (m_hashDirty)
        _computeHashUncheck();
      return m_hash;
    }

  private:

    void _computeHashUncheck() const;

  private:

    Container m_defines;

    mutable size_t m_hash = 0;
    mutable bool m_hashDirty = false;

  };

} // namespace vrm
