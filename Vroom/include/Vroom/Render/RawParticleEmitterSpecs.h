#pragma once

#include <span>
#include <unordered_map>
#include <utility>
#include <vector>

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

    inline Layout& operator=(Layout&& other) = delete;
    inline Layout(Layout&& other)            = delete;

    inline void addAttribute(EAttributeName attribName, size_t fieldCount)
    {
      m_attributes.emplace_back(std::make_pair(attribName, fieldCount));
    }

    inline const auto& getAttributes() const { return m_attributes; }

  private:

    std::vector<std::pair<EAttributeName, size_t>> m_attributes;
  };

public:

  RawParticleEmitterSpecs();
  ~RawParticleEmitterSpecs();

  RawParticleEmitterSpecs&
  operator=(const RawParticleEmitterSpecs& other)               = delete;
  RawParticleEmitterSpecs(const RawParticleEmitterSpecs& other) = delete;

  RawParticleEmitterSpecs& operator=(RawParticleEmitterSpecs&& other) = delete;
  RawParticleEmitterSpecs(RawParticleEmitterSpecs&& other)            = delete;

  void setupLayout(const Layout& layout);

  void setAttributeField(EAttributeName attribName, size_t fieldIndex,
                         const glm::vec4& value);

  inline std::span<glm::vec4 const> getData() const { return m_data; }

private:

  struct AttributeInfo
  {
    size_t offset;
    size_t size;
  };

private:

  std::vector<glm::vec4>                            m_data;
  std::unordered_map<EAttributeName, AttributeInfo> m_attributes;
};

} // namespace vrm
