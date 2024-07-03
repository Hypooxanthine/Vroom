// From vertex shader
// vec3 v_Normal;
// vec3 v_Position;
// vec2 v_TexCoord;

// From application
// vec3 u_ViewPosition;
// 
// struct PointLight
// {
//     vec3 position;
//     vec3 color;
//     float intensity;
//     float radius;
// };
// 
// layout(std430, binding = 0) buffer LightBlock
// {
//     int pointLightCount;
//     PointLight pointLights[];
// };

void PreFrag(out vec3 ambient, out vec3 diffuse, out vec3 specular, out float shininess);

vec4 ComputeColor()
{
    // Getting values from PreFrag shader
    vec3 ambient, diffuse, specular;
    float shininess;

    PreFrag(ambient, diffuse, specular, shininess);
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPosition - v_Position);

    vec3 shadeColor = ambient;
    
    for (int i = 0; i < pointLightCount; i++)
    {
        PointLight pointLight = pointLights[i];
        vec3 lightPos = vec3(pointLight.position[0], pointLight.position[1], pointLight.position[2]);
        vec3 lightColor = vec3(pointLight.color[0], pointLight.color[1], pointLight.color[2]);
        vec3 lightDir = normalize(lightPos - v_Position);

        // Diffuse factor
        float diff = max(dot(normal, lightDir), 0.f);

        // Specular factor
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), shininess);

        shadeColor += (diff * diffuse + spec * specular) * lightColor;
    }

    return vec4(shadeColor, 1.f);
}