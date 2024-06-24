// From vertex shader
// vec3 v_Normal;
// vec3 v_Position;
// vec2 v_TexCoord;

// From application
// vec3 u_ViewPosition;
// vec3 u_LightDirection;
// vec3 u_LightColor;

vec4 ComputeColor(out vec4 fragColor);

// Passthrough post processing shader
void PostFrag(in vec4 inFrag, out vec4 outFrag)
{
    outFrag = inFrag;
}