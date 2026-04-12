#pragma once

#include <bitset>

namespace vrm
{

  struct EMeshTag
  {
    enum Tag : size_t
    {
      eVisible,
      eShadowCaster,
      eCount
    };
  };

  using MeshTags = std::bitset<EMeshTag::eCount>;

}