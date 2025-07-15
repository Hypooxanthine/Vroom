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

  // Normal mapping
  #if defined(VRM_TEXTURE_u_normal) && defined(VRM_NORMAL_MAPPING)
  {
    vec3 N = normalize(vec3(u_Model * vec4(normal, 0.0)));
    vec3 T = normalize(vec3(u_Model * vec4(tangent, 0.0)));

  #ifdef VRM_REORTHOGONALIZE_TANGENT_SPACE
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
  #else
    vec3 B = normalize(vec3(u_Model * vec4(bitangent, 0.0)));
  #endif

    v_TBN = mat3(T, B, N);
  }
  #endif
}

#endif
