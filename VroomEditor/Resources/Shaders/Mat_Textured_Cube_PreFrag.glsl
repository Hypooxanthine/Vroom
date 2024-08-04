// From vertex shader
// vec3 v_Normal;
// vec3 v_Position;
// vec2 v_TexCoord;

// From application
// vec3 u_ViewPosition;
// vec3 u_LightDirection;
// vec3 u_LightColor;
// sampled2D u_Texture[1];

void PreFrag(out vec3 ambient, out vec3 diffuse, out vec3 specular, out float shininess)
{
    vec3 texturePx = texture(u_Texture[0], v_TexCoord).xyz;
    ambient = texturePx * 0.1f;
    diffuse = texturePx;
    specular = vec3(1.0, 1.0, 1.0);
    shininess = 16.0;
}