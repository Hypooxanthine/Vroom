

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void main()
{
  const uint thread = gl_GlobalInvocationID.x;

  if (thread >= u_maxParticleCount)
  {
    return;
  }

  const uint particleIndex = thread;


}