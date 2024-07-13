#version 460 core

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
    int indexOffset;
};

layout(std430, binding = 2) buffer ClusterInfoBlock
{
    int xCount;
    int yCount;
    int zCount;
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

    /*

    // Coordinates of the frag in VS for finding the right cluster
    uint zCoord = uint((log(abs(v_CameraDepth) / u_Near) * zCount) / log(u_Far / u_Near));
    vec2 clusterSizeXY = vec2(u_ViewportSize) / vec2(xCount, yCount);

    // Coordinates of the frag in NDC space for finding the right cluster
    uvec3 clusterCoords = ivec3(gl_FragCoord.xy / clusterSizeXY, zCoord);
    uint clusterIndex = clusterCoords.z * (yCount * xCount) + clusterCoords.y * (xCount) + clusterCoords.x;
    int lightsCount = clusters[clusterIndex].indexCount;
    
    vec4 lightComplexity = vec4( float(lightsCount) / pointLightCount, 0.0, 1.0 - float(lightsCount) / pointLightCount, 1.0);

    finalColor = mix(processedColor, lightComplexity, 0);

    */

    finalColor = processedColor;
}