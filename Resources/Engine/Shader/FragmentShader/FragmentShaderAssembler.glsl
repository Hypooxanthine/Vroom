#version 450 core

// From vertex shader
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;
in float v_CameraDepth;

// From application
uniform vec3 u_ViewPosition;
uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform float u_Near;
uniform float u_Far;
uniform uvec2 u_ViewportSize;

struct PointLight
{
    float position[3];
    float color[3];
    float intensity;
    float radius;
};

layout(std430, binding = 0) buffer LightBlock
{
    uint pointLightCount;
    PointLight pointLights[];
};

struct Cluster
{
    vec4 minAABB_VS;
    vec4 maxAABB_VS;
    uint indexCount;
    uint lightIndices[100];
};

layout(std430, binding = 1) buffer ClusterInfoBlock
{
    uint xCount;
    uint yCount;
    uint zCount;
    Cluster clusters[];
};

// Function code found on Victor Gordan's Youtube video: https://www.youtube.com/watch?v=3xGKu4T4SCU
float linearizeDepth(float depth)
{
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - (depth * 2.0 - 1.0) * (u_Far - u_Near));
}

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