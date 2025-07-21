#ifndef _RENDERFULLSCREEN_FRAGMENT_GLSL_
#define _RENDERFULLSCREEN_FRAGMENT_GLSL_

layout(location = 0) out vec4 finalColor;

vec4 RenderFullscreen_impl();

void main()
{
  finalColor = RenderFullscreen_impl();
}

vec2 GetTexCoords()
{
  return v_TexCoord;
}

#endif // _RENDERFULLSCREEN_FRAGMENT_GLSL_
