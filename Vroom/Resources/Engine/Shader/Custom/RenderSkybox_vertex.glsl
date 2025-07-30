
out vec3 texCoords;

void main()
{
  texCoords = position;
  gl_Position = u_viewProjection * vec4(position, 1.0);
}