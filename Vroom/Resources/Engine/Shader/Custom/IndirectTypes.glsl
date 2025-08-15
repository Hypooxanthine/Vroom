#ifndef INDIRECT_TYPES_GLSL
#define INDIRECT_TYPES_GLSL

struct DrawElementsIndirectCommand
{
  uint count;
  uint instanceCount;
  uint firstIndex;
  int  baseVertex;
  uint baseInstance;
};

#endif // PARTICLE_TYPES_GLSL