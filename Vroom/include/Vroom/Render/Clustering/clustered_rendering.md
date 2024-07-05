# Clustered rendering {#clustered_rendering}

This page is about how clustered rendering (or forward+ rendering, or tiled rendering) is implemented in Vroom.

## Forward rendering

### Abstract

Forward rendering is a lighting technique allowing the use of multiple lights. When shading a fragment in the fragment shader, we will accumulate the color resulting from the application of every light. Obviously, a advantage of it is the possibility of using as many lights as needed. The downside of this technique is the enormous performance issue when using too many lights: each fragment will need to be shaded as many times as lights in the scene.

This problem will be greatly reduced with the implementation of clustered rendering.

### Implementation

In a scene, we can create as many entities as needed with a @ref vrm::PointLightComponent attached to them.

At the rendering phase, each PointLightComponent is submitted to the @ref vrm::Renderer. There, it is submitted one last time to the @ref vrm::LightRegistry. The light registry keeps track of every point lights on the scene, frame per frame.
- When a submitted light was not registered previously, it will allocate (if needed) enough of memory on the GPU to store the new light data, through a @ref vrm::SSBOPointLightData. This class is convenient for sending point light data via a [SSBO](https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object), because the layout is meant to be exactly the same as requested in the fragment shader.
- When a submitted light already existed in the previous frame, we only need to update it.
- At the end of the frame, the registry knows all the lights that have not been submitted. It means that those were removed. In this scenario, the registry does not shrink the SSBO, but will only keep track of this free memory. When a light is created, the registry will assign it to a free memory place instead of requesting more space to the @ref vrm::DynamicSSBO.

DynamicSSBO is an OpenGL abstraction class that will keep track of the data stored on the GPU, and the data that is not used. When the registry asks the DynamicSSBO to store some data, at a certain memory address, it will create a new SSBO with more storage if needed, and it will just write into the SSBO if the storage is enough. DynamicSSBO uses @ref ShaderStorageBufferObject, which is a very simple OpenGL SSBO abstraction.

From there we have a list of point lights sent to the fragment shader. But because of the possible fragmentation of the lights SSBO, we cannot just iterate through it from the fragment shader. At the beginning, fragmentation was not possible, the SSBO was reallocated entirely each frame (for testing purposes), and it was possible to iterate through it. But now, we'll need an indices SSBO, which will reference valid light addresses. This could be seen as a GPU memory loss, but for clustered rendering, we'll need a light indices SSBO anyway, so I think the fragmentation is no problem.

NB: the fragmentation is always of a multiple of a point light size, with the same offset. That means, if a point light data is sized 4 bytes, every point lights have an adress in the form offset + 4*n (the offset is the size of some extra data that could be sent in the point lights SSBO).

## Clustering (or tiling, or ...)

### Abstract

Clustering is a really great improvement of forward rendering. The idea is:
- Separate the view space into clusters (or blocks).
- When a culled light is added (tipically, a point light with a culling radius), check for all clusters affected by this light, and register it to these clusters.
- Pass this data to the fragment shader.
- When shading a fragment, check for which cluster it belongs to, and only accumulate the color coming from lights that have an influence on this cluster.

For lights that aren't culled (i.e. a directional light), we'll need to take them in account for every fragments (those can't benefit from clustering).

### Implementation

@todo Finish documenting clustered rendering.

#### Clustering the view space

#### How to check if a light has influence on any cluster ? (maths)

#### Layouting the data for the GPU

#### Using the data from the fragment shader
