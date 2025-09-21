#include "Vroom/Render/MaterialDefines.h"

#include <glm/gtx/hash.hpp>

#include "Vroom/Core/Assert.h"

using namespace vrm;

MaterialDefines::MaterialDefines() {}

MaterialDefines::~MaterialDefines() {}

void MaterialDefines::add(const std::string& define, std::string&& value)
{
  VRM_ASSERT_MSG(!m_defines.contains(define), "Define {} is already set",
                 define);

  m_defines.insert(std::make_pair(define, std::move(value)));
}

void MaterialDefines::add(const MaterialDefines& defines)
{
  for (auto&& [define, value] : defines)
  {
    // Adding each define from other defines
    add(define, value);
  }
}
