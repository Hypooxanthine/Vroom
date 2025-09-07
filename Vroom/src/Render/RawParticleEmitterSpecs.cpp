#include "Vroom/Render/RawParticleEmitterSpecs.h"
#include <cstddef>
#include <unordered_map>

#include "Vroom/Core/Assert.h"

using namespace vrm;

RawParticleEmitterSpecs::RawParticleEmitterSpecs() {}

RawParticleEmitterSpecs::~RawParticleEmitterSpecs() {}

void RawParticleEmitterSpecs::setupLayout(const Layout& layout)
{
  m_data.clear();

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

  m_data.resize(dataSize, glm::vec4{});
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

  size_t dataIndex     = attribInfo.offset + fieldIndex;
  m_data.at(dataIndex) = value;
}
