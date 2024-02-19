#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_Model;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(position, 1.0);
	v_TexCoord = texCoord;
}