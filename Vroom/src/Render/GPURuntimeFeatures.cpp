#include "Vroom/Render/GPURuntimeFeatures.h"

#include "Vroom/Render/Abstraction/GLCall.h"

using namespace vrm;

#define CAST_SET(target, src) target = static_cast<decltype(target)>(src)

void GPURuntimeFeatures::query()
{
  GLint integerv;

  glGetIntegerv(GL_MAX_SAMPLES, &integerv);
  CAST_SET(maxMSAA, integerv);
  
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &integerv);
  CAST_SET(maxArrayTextureLayers, integerv);
}
