
out vec3 texCoords;

void main()
{
  texCoords = position;
  gl_Position = (u_projection * u_view * vec4(position, 1.f)).xyww;
}