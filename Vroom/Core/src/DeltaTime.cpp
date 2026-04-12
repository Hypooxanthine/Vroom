#include "Core/DeltaTime.h"

using namespace vrm;

namespace
{
  static DeltaTime GLOBAL_DELTA_TIME;
}

const DeltaTime& DeltaTime::Get()
{
  return GLOBAL_DELTA_TIME;
}

void DeltaTime::Set(const DeltaTime& dt)
{
  GLOBAL_DELTA_TIME = dt;
}
