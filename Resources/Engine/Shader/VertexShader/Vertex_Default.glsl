#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out float v_CameraDepth;

void main()
{
	vec4 worldPosition = u_Model * vec4(position, 1.0);
	vec4 cameraPosition = u_View * worldPosition;

	gl_Position = u_Projection * cameraPosition;
	
	v_Position = vec3(u_Model * vec4(position, 1.0));
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * normal);
	v_TexCoord = texCoord;
	v_CameraDepth = -cameraPosition.z;
}