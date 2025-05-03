#ifndef _MATERIALBASE_VERTEX_GLSL_
#define _MATERIALBASE_VERTEX_GLSL_

void main()
{

	vec4 worldSpacePosition = u_Model * vec4(position, 1.0);
	vec4 viewSpacePosition = u_View * worldSpacePosition;

	gl_Position = u_Projection * viewSpacePosition;

#if defined(VRM_SHADOW_PASS)
  return;
#endif // defined(VRM_SHADOW_PASS)
	
	v_Position = worldSpacePosition.xyz;
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * normal);
	v_TexCoord = texCoord;
	v_CameraDepth = -viewSpacePosition.z;
}

#endif
