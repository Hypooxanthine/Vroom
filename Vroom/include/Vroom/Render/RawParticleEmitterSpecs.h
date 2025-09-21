#pragma once

#include <cstddef>
#include <span>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Vroom/Tools/Maths.h"
#include "glm/glm.hpp"

namespace vrm
{

class RawParticleEmitterSpecs
{
public:

  enum class EAttributeName : size_t
  {
    ePosition = 0,
    eColor,
    eScale
  };

  class Layout
  {
  public:

    inline Layout()  = default;
    inline ~Layout() = default;

    inline Layout& operator=(const Layout& other) = delete;
    inline Layout(const Layout& other)            = delete;

    inline Layout& operator=(Layout&& other) = default;
    inline Layout(Layout&& other)            = default;

    inline void addAttribute(EAttributeName attribName, size_t fieldCount)
    {
      m_attributes.emplace_back(std::make_pair(attribName, fieldCount));
    }

    inline const auto& getAttributes() const { return m_attributes; }

  private:

    std::vector<std::pair<EAttributeName, size_t>> m_attributes;
  };

  struct EmitterData
  {
    struct Header
    {
      float lifeTime;
      float emitRate;
    };

    Header                 header;
    std::vector<glm::vec4> attributes;

    inline static constexpr size_t HeaderSize =
      maths::NextPowerInclusive(sizeof(Header), 16);
  };

public:

  RawParticleEmitterSpecs();
  ~RawParticleEmitterSpecs();

  RawParticleEmitterSpecs&
  operator=(const RawParticleEmitterSpecs& other)               = delete;
  RawParticleEmitterSpecs(const RawParticleEmitterSpecs& other) = delete;

  RawParticleEmitterSpecs& operator=(RawParticleEmitterSpecs&& other) = default;
  RawParticleEmitterSpecs(RawParticleEmitterSpecs&& other)            = default;

  void reset();

  void setupLayout(const Layout& layout);

  inline void setHeader(const EmitterData::Header& header)
  {
    m_data.header = header;
  }

  void setAttributeField(EAttributeName attribName, size_t fieldIndex,
                         const glm::vec4& value);

  void copyData(std::span<std::byte> dst);

  size_t getDataSize() const;
  size_t getStatesRequiredSize() const;

private:

  struct AttributeInfo
  {
    size_t offset;
    size_t size;
  };

private:

  EmitterData                                       m_data;
  std::unordered_map<EAttributeName, AttributeInfo> m_attributes;
};

} // namespace vrm
