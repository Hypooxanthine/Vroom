#include "Vroom/Render/RawParticleEmitterSpecs.h"
#include <cstddef>
#include <cstring>
#include <unordered_map>

#include "Vroom/Core/Assert.h"
#include "glm/fwd.hpp"

using namespace vrm;

RawParticleEmitterSpecs::RawParticleEmitterSpecs() {}

RawParticleEmitterSpecs::~RawParticleEmitterSpecs() {}

void RawParticleEmitterSpecs::reset()
{
  m_data.attributes.clear();
  m_attributes.clear();
}

void RawParticleEmitterSpecs::setupLayout(const Layout& layout)
{
  m_data.attributes.clear();

  size_t dataSize = 0;

  for (const auto& [attribName, fieldCount] : layout.getAttributes())
  {
    VRM_ASSERT_MSG(!m_attributes.contains(attribName),
                   "Attribute {} has already been added", (size_t)attribName);

    AttributeInfo& attribute = m_attributes[attribName];

    attribute.offset = dataSize;
    attribute.size   = fieldCount;

    dataSize += attribute.size;
  }

  m_data.attributes.resize(dataSize, glm::vec4{});
}

void RawParticleEmitterSpecs::setAttributeField(EAttributeName   attribName,
                                                size_t           fieldIndex,
                                                const glm::vec4& value)
{
  auto attribIt = m_attributes.find(attribName);
  VRM_ASSERT_MSG(attribIt != m_attributes.end(),
                 "This attribute was not declared");

  const AttributeInfo& attribInfo = attribIt->second;
  VRM_ASSERT_MSG(attribInfo.size > fieldIndex, "Field index is out of bounds");

  size_t dataIndex                = attribInfo.offset + fieldIndex;
  m_data.attributes.at(dataIndex) = value;
}

void RawParticleEmitterSpecs::copyData(std::span<std::byte> dst)
{
  VRM_ASSERT_MSG(dst.size_bytes() >= getDataSize(),
                 "Destination buffer is too small. Size: {}, expected: {}",
                 dst.size_bytes(), getDataSize());

  std::memcpy(dst.data(), &m_data.header, sizeof(m_data.header));
  std::memcpy(dst.data() + EmitterData::HeaderSize, m_data.attributes.data(),
              m_data.attributes.size() * sizeof(glm::vec4));
}

size_t RawParticleEmitterSpecs::getDataSize() const
{
  return EmitterData::HeaderSize + m_data.attributes.size() * sizeof(glm::vec4);
}

size_t RawParticleEmitterSpecs::getStatesRequiredSize() const
{
  static const constinit size_t headerSize =
    maths::NextPowerInclusive(sizeof(glm::uint)  // Active
                                + sizeof(float)  // Ellapsed time
                                + sizeof(float), // Max lifetime
                              16);

  // Each attribute has a spawn and death value
  return headerSize + m_attributes.size() * sizeof(glm::vec4) * 2;
}
