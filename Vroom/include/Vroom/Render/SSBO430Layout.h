#pragma once

#include <cstddef>

#include "Vroom/Tools/Maths.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"

namespace vrm::render
{

template <typename T>
struct SSBO430Traits;

/**
 * @brief Helper class used for matching OpenGL SSBOs std430 layout. It allows
 * pushing attributes and gives their location in the struct and the whole
 * struct size with or without padding.
 *
 */
class SSBO430Layout
{
public:

  struct Attrib
  {
    friend class SSBO430Layout;

    constexpr Attrib() = default;
    ~Attrib()          = default;

    constexpr Attrib& operator=(const Attrib& other) = default;
    constexpr Attrib(const Attrib& other)            = default;

    constexpr Attrib& operator=(Attrib&& other) = default;
    constexpr Attrib(Attrib&& other)            = default;

    inline constexpr size_t getAlignment() const { return alignment; }
    inline constexpr size_t getSize() const { return size; }

  private:

    size_t alignment;
    size_t size;
  };

public:

  constexpr SSBO430Layout() = default;
  ~SSBO430Layout()          = default;

  constexpr SSBO430Layout& operator=(const SSBO430Layout& other) = default;
  constexpr SSBO430Layout(const SSBO430Layout& other)            = default;

  constexpr SSBO430Layout& operator=(SSBO430Layout&& other) = default;
  constexpr SSBO430Layout(SSBO430Layout&& other)            = default;

  constexpr void reset()
  {
    m_elementSize = 0;
    m_alignment   = 1;
  }

  constexpr size_t getRawSize() const { return m_elementSize; }

  constexpr size_t getAlignedSize() const
  {
    return maths::NextPowerMultipleInclusive(m_elementSize, m_alignment);
  }

  /**
   * @brief Pushes an attribute into the layout.
   *
   * @tparam T The type of the attribute to push.
   * @return constexpr size_t The location of the attribute in the struct,
   * starting from zero.
   */
  template <typename T>
  constexpr inline Attrib push()
  {
    return _push(SSBO430Traits<T>::size, SSBO430Traits<T>::alignment);
  }

private:

  inline constexpr Attrib _push(size_t elemSize, size_t alignment);

private:

  size_t m_elementSize = 0;
  size_t m_alignment   = 0;
};

template <>
struct SSBO430Traits<glm::float32>
{
  static constexpr size_t size      = 4;
  static constexpr size_t alignment = 4;
};
template <>
struct SSBO430Traits<glm::int32> : SSBO430Traits<float>
{};
template <>
struct SSBO430Traits<glm::uint32> : SSBO430Traits<float>
{};

template <>
struct SSBO430Traits<glm::vec2>
{
  static constexpr size_t size      = 8;
  static constexpr size_t alignment = 8;
};

template <>
struct SSBO430Traits<glm::vec3>
{
  static constexpr size_t size      = 12;
  static constexpr size_t alignment = 16;
};

template <>
struct SSBO430Traits<glm::vec4>
{
  static constexpr size_t size      = 16;
  static constexpr size_t alignment = 16;
};

template <>
struct SSBO430Traits<glm::mat3>
{
  // Each column is aligned as a vec4
  static constexpr size_t size      = 3 * 4 * 4;
  static constexpr size_t alignment = 16;
};

template <>
struct SSBO430Traits<glm::mat4>
{
  static constexpr size_t size      = 4 * 4 * 4;
  static constexpr size_t alignment = 16;
};

inline constexpr SSBO430Layout::Attrib SSBO430Layout::_push(size_t elemSize,
                                                            size_t alignment)
{
  size_t location = maths::NextPowerMultipleInclusive(m_elementSize, alignment);
  m_elementSize   = location + elemSize;
  m_alignment     = glm::max(m_alignment, alignment);

  Attrib outAttrib;
  outAttrib.alignment = alignment;
  outAttrib.size      = elemSize;
  return outAttrib;
}

} // namespace vrm::render
