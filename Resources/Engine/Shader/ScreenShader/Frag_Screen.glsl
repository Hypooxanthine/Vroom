#version 460 core
  
in vec2 v_TexCoord;

uniform sampler2D u_ScreenTexture;

layout(location = 0) out vec4 finalColor;

void main()
{ 
    finalColor = texture(u_ScreenTexture, v_TexCoord);
}