#include "Vroom/Render/MaterialDefines.h"

#include "Vroom/Core/Assert.h"
#include <glm/gtx/hash.hpp>

using namespace vrm;

MaterialDefines::MaterialDefines()
{

}

MaterialDefines::~MaterialDefines()
{

}

void MaterialDefines::add(const std::string& define, std::string&& value)
{
  VRM_ASSERT_MSG(!m_defines.contains(define), "Define {} is already set", define);

  m_defines.insert(std::make_pair(define, std::move(value)));
}

void MaterialDefines::_computeHashUncheck() const
{
  m_hash = 0;
  auto hasher = std::hash<std::string>();
  for (const auto& [define, value] : m_defines)
  {
    glm::detail::hash_combine(m_hash, hasher(define));
    glm::detail::hash_combine(m_hash, hasher(value));
  }
}
