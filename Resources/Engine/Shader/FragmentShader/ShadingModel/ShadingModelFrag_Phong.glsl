// From vertex shader
// vec3 v_Normal;
// vec3 v_Position;
// vec2 v_TexCoord;

// From application
// vec3 u_ViewPosition;
// vec3 u_LightDirection;
// vec3 u_LightColor;

void PreFrag(out vec3 ambient, out vec3 diffuse, out vec3 specular, out float shininess);

vec4 ComputeColor()
{
    // Getting values from PreFrag shader
    vec3 ambient, diffuse, specular;
    float shininess;

    PreFrag(ambient, diffuse, specular, shininess);

    // Diffuse factor
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(-u_LightDirection);
    float diff = max(dot(normal, lightDir), 0.f);

    // Specular factor    
    vec3 viewDir = normalize(u_ViewPosition - v_Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), shininess);


    vec3 result = (ambient + diff * diffuse + spec * specular) * u_LightColor;

    return vec4(result, 1.f);
}