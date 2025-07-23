

vec4 RenderFullscreen_impl()
{
  vec3 hdrColor = texture(u_hdrTex, GetTexCoords()).rgb;

#ifdef VRM_BLOOM_ENABLE
  hdrColor += (u_bloomIntensity * texture(u_blurredBrightness, GetTexCoords()).rgb);
#endif

  vec3 mapped = vec3(1.0) - exp(-hdrColor * u_exposure);

  mapped = pow(mapped, vec3(1.f / 2.2f)); // gamma correction

  return vec4(mapped, 1.f);
}
