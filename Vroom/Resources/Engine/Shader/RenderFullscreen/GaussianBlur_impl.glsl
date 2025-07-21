
const float rightGaussianVals[] = { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };

vec2 texelSize;

vec4 BlurHorizontally()
{
  vec3 fragColor = rightGaussianVals[0] * texture(u_inputTexture, GetTexCoords()).rgb;

  for (int i = 1; i < 5; ++i)
  {
    fragColor = fragColor + rightGaussianVals[i] * texture(u_inputTexture, GetTexCoords() + vec2(i * texelSize.x, 0.f)).rgb;
    fragColor = fragColor + rightGaussianVals[i] * texture(u_inputTexture, GetTexCoords() - vec2(i * texelSize.x, 0.f)).rgb;
  }

  return vec4(fragColor, 1.f);
}

vec4 BlurVertically()
{
  vec3 fragColor = rightGaussianVals[0] * texture(u_inputTexture, GetTexCoords()).rgb;

  for (int i = 1; i < 5; ++i)
  {
    fragColor = fragColor + rightGaussianVals[i] * texture(u_inputTexture, GetTexCoords() + vec2(0.f, i * texelSize.x)).rgb;
    fragColor = fragColor + rightGaussianVals[i] * texture(u_inputTexture, GetTexCoords() - vec2(0.f, i * texelSize.x)).rgb;
  }

  return vec4(fragColor, 1.f);
}

vec4 RenderFullscreen_impl()
{
  texelSize = 1.f / textureSize(u_inputTexture, 0);

  if (u_vertical != 0)
  {
    return BlurVertically();
  }
  else
  {
    return BlurHorizontally();
  }
}
