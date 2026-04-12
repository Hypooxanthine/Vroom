#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace vrm
{

class ScriptEngine;
class RuntimeScriptLibrary;

class ScriptHandle
{
public:

  friend RuntimeScriptLibrary;
  friend ScriptEngine;

public:

  bool isValid() const
  {
    return m_id != s_invalidId && m_generation != s_invalidGeneration;
  }

  size_t getId() const
  {
    return m_id;
  }

  uint32_t getGeneration() const
  {
    return m_generation;
  }

  uint32_t getLibraryGeneration() const
  {
    return m_libraryGeneration;
  }

private:

  void setId(size_t id)
  {
    m_id = id;
  }

  void setGeneration(uint32_t generation)
  {
    m_generation = generation;
  }

  void setLibraryGeneration(uint32_t generation)
  {
    m_libraryGeneration = generation;
  }

private:

  static constexpr size_t   s_invalidId                = std::numeric_limits<size_t>::max();
  static constexpr uint32_t s_invalidGeneration        = std::numeric_limits<uint32_t>::max();
  static constexpr uint32_t s_invalidLibraryGeneration = std::numeric_limits<uint32_t>::max();

  size_t   m_id                = s_invalidId;
  uint32_t m_generation        = s_invalidGeneration;
  uint32_t m_libraryGeneration = s_invalidLibraryGeneration;
};

} // namespace vrm
