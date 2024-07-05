#version 460 core

#define MAX_LIGHTS_PER_CLUSTER 50

// From vertex shader
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;

// From application
uniform vec3 u_ViewPosition;

struct PointLight
{
    float position[3]; // We need to use an array of floats instead of a vec3 because of the alignment of std430 (rounded up to 4)
    float color[3];
    float intensity;
    float radius;
};

layout(std430, binding = 0) buffer LightBlock
{
    int pointLightCount;
    PointLight pointLights[];
};

layout(std430, binding = 1) buffer ClusterIndicesBlock
{
    int indices[];
};

struct Cluster
{
    int indexCount;
    int indexeOffset;
};

layout(std430, binding = 2) buffer ClusterInfoBlock
{
    int xSize;
    int ySize;
    int zSize;
    Cluster clusters[];
};

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