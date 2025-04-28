#ifndef _MATERIALBASE_VERTEX_GLSL_
#define _MATERIALBASE_VERTEX_GLSL_

void main()
{

	vec4 worldPosition = u_Model * vec4(position, 1.0);
	vec4 cameraPosition = u_View * worldPosition;

	gl_Position = u_Projection * cameraPosition;

#if defined(VRM_SHADOW_PASS)
  return;
#endif // defined(VRM_SHADOW_PASS)
	
	v_Position = worldPosition.xyz;
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * normal);
	v_TexCoord = texCoord;
	v_CameraDepth = -cameraPosition.z;
}

#endif
