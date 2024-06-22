#version 460 core

in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;

uniform vec3 u_ViewPosition;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;

struct UniformComponent
{
    bool useTexture;
    vec3 value;
    sampler2D texture;
};

uniform UniformComponent u_Ambient;
uniform UniformComponent u_Diffuse;
uniform UniformComponent u_Specular;
uniform float u_Shininess;

layout(location = 0) out vec4 color;

void main()
{
    // Ambient
    vec3 ambient = u_Ambient.useTexture ? texture(u_Ambient.texture, v_TexCoord).rgb : u_Ambient.value;

    // Diffuse
    vec3 diffuse = u_Diffuse.useTexture ? texture(u_Diffuse.texture, v_TexCoord).rgb : u_Diffuse.value;
    
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(-u_LightDirection);
    float diff = max(dot(normal, lightDir), 0.f);

    // Specular
    vec3 specular = u_Specular.useTexture ? texture(u_Specular.texture, v_TexCoord).rgb : u_Specular.value;
    
    vec3 viewDir = normalize(u_ViewPosition - v_Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), u_Shininess);

    vec3 result = (ambient + diff * diffuse + spec * specular) * u_LightColor;

    color = vec4(result, 1.f);
}