#ifndef _RENDERFULLSCREEN_FRAGMENT_GLSL_
#define _RENDERFULLSCREEN_FRAGMENT_GLSL_

layout(location = 0) out vec4 finalColor;

void main()
{
  vec3 hdrColor = texture(u_hdrTex, v_TexCoord).rgb;

  vec3 mapped = vec3(1.0) - exp(-hdrColor * u_exposure);

  mapped = pow(mapped, vec3(1.f / u_gamma));

  finalColor = vec4(mapped, 1.f);
}

#endif // _RENDERFULLSCREEN_FRAGMENT_GLSL_
