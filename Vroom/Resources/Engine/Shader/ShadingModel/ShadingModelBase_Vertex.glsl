#ifndef _SHADINGMODELBASE_VERTEX_GLSL_
#define _SHADINGMODELBASE_VERTEX_GLSL_

void EntryPoint()
{
	vec4 worldSpacePosition = u_Model * vec4(position, 1.0);
	vec4 viewSpacePosition = u_View * worldSpacePosition;

	gl_Position = u_Projection * viewSpacePosition;

#if defined(VRM_SHADOW_PASS)
  return;
#endif // defined(VRM_SHADOW_PASS)
	
	v_Position = worldSpacePosition.xyz;
  mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	v_Normal = normalize(normalMatrix * normal);
	v_TexCoord = texCoord;
	v_CameraDepth = -viewSpacePosition.z;

  // Normal mapping
  #if defined(VRM_TEXTURE_u_normal) && defined(VRM_NORMAL_MAPPING)
  {
    vec3 N = normalize(normalMatrix * normal);
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);

  #ifdef VRM_REORTHOGONALIZE_TANGENT_SPACE
    T = normalize(T - dot(T, N) * N); // Reortho T in respect of N
    B = normalize(B - dot(B, N) * N - dot(B, T) * T); // Reortho B in respect of N and T
  #endif

    v_TBN = mat3(T, B, N);
  }
  #endif
}

#endif
