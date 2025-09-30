#pragma once

#include <string>
#include <unordered_map>

#include "Vroom/Render/SSBO430Layout.h"

namespace vrm::render
{

class SSBOAttributes
{
public:

  SSBOAttributes()  = default;
  ~SSBOAttributes() = default;

  SSBOAttributes& operator=(const SSBOAttributes& other) = default;
  SSBOAttributes(const SSBOAttributes& other)            = default;

  SSBOAttributes& operator=(SSBOAttributes&& other) = default;
  SSBOAttributes(SSBOAttributes&& other)            = default;

  inline void setAttribute(const std::string&           name,
                           const SSBO430Layout::Attrib& attrib)
  {
    m_attribs[name] = attrib;
  }

  inline const SSBO430Layout::Attrib&
  getAttribute(const std::string& name) const
  {
    return m_attribs.at(name);
  }

  inline const SSBO430Layout::Attrib& operator[](const std::string& name) const
  {
    return getAttribute(name);
  }

private:

  std::unordered_map<std::string, SSBO430Layout::Attrib> m_attribs;
};

} // namespace vrm::render
