// From vertex shader
// vec3 v_Normal;
// vec3 v_Position;
// vec2 v_TexCoord;

// From application
// vec3 u_ViewPosition;
// vec3 u_LightDirection;
// vec3 u_LightColor;

// Default phong values
void PreFrag(out vec3 ambient, out vec3 diffuse, out vec3 specular, out float shininess)
{
    ambient = vec3(0.2, 0.2, 0.2);
    diffuse = vec3(0.5, 0.5, 0.5);
    specular = vec3(0.5, 0.5, 0.5);
    shininess = 1.0;
}