
out vec4 finalColor;

in vec3 texCoords;

void main()
{    
  finalColor = texture(u_cubemap, texCoords);
}