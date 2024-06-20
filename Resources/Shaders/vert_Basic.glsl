#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProjection;

out vec3 v_Normal;
out vec3 v_Position;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(position, 1.0);
	v_Position = vec3(u_Model * vec4(position, 1.0));
	v_Normal = normal;
}