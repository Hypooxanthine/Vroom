#version 460 core

// From vertex shader
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;

// From application
uniform vec3 u_ViewPosition;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;

// For Vroom shader preprocessor
#include Sampler2DUniform
#include PreFragShader
#include ShadingModelShader
#include PostFragShader

layout(location = 0) out vec4 finalColor;

void main()
{
    // Compute color from shading model
    vec4 shadeColor = ComputeColor();

    // Apply post processing shader
    vec4 processedColor;
    PostFrag(shadeColor, processedColor);

    finalColor = processedColor;
}