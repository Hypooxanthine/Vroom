

void main()
{
  gl_Position = u_viewProj * g_particleInstanceData[gl_InstanceID].modelMatrix * vec4(position, 1.f);
  v_color = g_particleInstanceData[gl_InstanceID].color;
}
