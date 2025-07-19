#ifndef _RENDERFULLSCREEN_VERTEX_GLSL_
#define _RENDERFULLSCREEN_VERTEX_GLSL_

const vec2 s_vertices[3] = {
  vec2(0.f, 0.f),
  vec2(2.f, 0.f),
  vec2(0.f, 2.f)
};

void main()
{
  v_TexCoord = s_vertices[gl_VertexID];
  gl_Position = vec4(v_TexCoord * 2.f - 1.f, 0.f, 1.f);
}

#endif // _RENDERFULLSCREEN_VERTEX_GLSL_
