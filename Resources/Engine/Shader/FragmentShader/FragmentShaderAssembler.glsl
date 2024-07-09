#version 460 core

#define MAX_LIGHTS_PER_CLUSTER 50

// From vertex shader
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;
in vec4 v_HomogeneousNDCPosition;
in float v_CameraDepth;

// From application
uniform vec3 u_ViewPosition;
uniform mat4 u_ViewProjection;
uniform float u_Near;
uniform float u_Far;

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

    // Coordinates of the frag in NDC space for finding the right cluster
    vec3 NDCPosition = v_HomogeneousNDCPosition.xyz / v_HomogeneousNDCPosition.w;
    vec3 NormalizedLinearNDCPosition = vec3(
        NDCPosition.xy * 0.5 + 0.5,
        linearizeDepth(gl_FragCoord.z) / u_Far
    );
    
    ivec3 clusterCoords = ivec3(   NormalizedLinearNDCPosition * vec3(xCount, yCount, zCount)   );
    int clusterIndex = clusterCoords.z * yCount * xCount + clusterCoords.y * xCount + clusterCoords.x;
    int lightsCount = clusters[clusterIndex].indexCount;

    // Trying to convert to linearized NDC space

    //finalColor = vec4(clusterCoords / 7.0, 1.0);
    //finalColor = vec4(vec3(NormalizedLinearNDCPosition.z), 1.0);
    
    //vec4 lightComplexity = vec4( float(lightsCount) / pointLightCount, 0.0, 1.0 - float(lightsCount) / pointLightCount, 1.0);

    //finalColor = mix(processedColor, lightComplexity, 0);

    finalColor = processedColor;
}