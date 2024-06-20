#version 460 core

layout(location = 0) out vec4 color;

uniform mat4 u_View;

in vec3 v_Normal;
in vec3 v_Position;

void main()
{
    vec3 lightDirection = normalize(vec3(-0.5, -1.0, -0.3));
    vec3 l = normalize(-lightDirection);

    float diffuse = max(dot(v_Normal, l), 0.0);

    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuseColor = diffuse * baseColor;

    color = vec4(diffuseColor, 1.f);
}