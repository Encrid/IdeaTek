# BM-IdeaTek-Rendering-Vulkan

The render module of IdeaTek on Vulkan: it culls, picks the levels and lights the meshes of each camera.

## Install

The release of `BM-IdeaTek-Rendering-Vulkan` has the tag `rendering_vulkan-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Rendering-Vulkan.zip` and `sha256.txt`. `BM-IdeaTek-Rendering-Vulkan.zip` has a folder for each target.
In `BM-IdeaTek-Rendering-Vulkan`, each folder holds the module `rendering_vulkan.dll`, `rendering_vulkan.so` or `rendering_vulkan.dylib`, and the notices of glslang, volk and the headers of Vulkan.
Each folder of `BM-IdeaTek-Rendering-Vulkan.zip` also holds this readme and the licence.

The shaders are in the module, so no other file goes beside it.
A project names the zip of `BM-IdeaTek-Rendering-Vulkan` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_rendering_vulkan:render@0.1.0`.
Its role is the render module: `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The roles*.
It draws through the device and the window images of the GPU module: `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`.

### What the module draws

The module draws each camera that is active, into its window or its texture, in the order of `order`.
It draws each entity with `ideatek:MeshInstance` or `ideatek:MeshInstances` and `ideatek:Visibility` whose groups meet the groups of the camera.
It lights them with each `ideatek:Light` whose `ideatek:Affects` meets their groups. It reads `ideatek:Material` and the tag `ideatek:MaterialUnlit`.
It applies `ideatek:Exposure` and `ideatek:Tonemap` of the camera. A camera without them gets no exposure and the curve `IDEATEK_TONEMAP_CURVE_LINEAR`.

It answers `ideatek:CaptureRequest`: `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The captures*.
It writes `ideatek:RenderStats`: `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The counts of a frame*.

This version draws no shadow, no global illumination, no sky, no fog, no decal and no reflection.
It applies no effect of a camera but the exposure and the tonemap.
It reads no extension of a material but the unlit tag, as the contract allows. The side of a mesh that no light reaches is black.
The log gets 1 line for the first entity of each type that it does not draw.

### The steps of a frame

1. The CPU uploads only what changed. It finds the resources by `revision`, and the places by `ideatek:WorldMoves`.
   It also uploads the entities that enter, and forgets the entities that leave.
2. A compute pass tests each instance against the view of each camera.
   It picks the level of each sub-mesh by its error on the screen, and writes the commands of the draws.
   So the CPU records a few draws for each pipeline, whatever the count of instances.
3. With `occlusion`, the pass first draws the instances that the last frame drew, and builds a pyramid of the depth.
   Then it tests the other instances against that pyramid.
4. A depth pass writes the depth. A compute pass puts the lights into the cells of the view. The forward pass lights each pixel from the lights of its cell.
5. The last pass applies the exposure and the tonemap, and writes the target.

The places of the world are in `double`. The module splits each origin into 2 `float` values when it uploads it.
The compute pass then subtracts the place of the camera, with 2 `float` values for each number. So the view is exact far from the origin.
When the camera goes farther than `origin_distance` from the origin of the module, the module moves that origin and uploads each instance again.

### The light

The surface follows the sample of glTF 2.0, Appendix B.3. Points and spots fall off and shape their cones as `KHR_lights_punctual` tells.
`diffuse`, `specular` and `shading` make the light cheaper or simpler for all materials: *The settings*.

### The shaders

The shaders are GLSL in the module. glslang turns them into SPIR-V at the start, before the first round.
When `shader_folder` names a folder of the content storage, the module asks the files part for each shader of that folder, and watches it.
A shader that comes back replaces the shader of the module with the same name. A shader that does not compile keeps the shader of the module, and the log gets 1 line.

A setting that changes in play is a constant of the pipelines. The module builds the new pipelines on a thread of its own.
Until a pipeline is ready, the module draws with a generic pipeline that reads the setting in each draw. So a change in play does not stop a frame.
A setting that changes the size of a buffer takes effect at the next load of the module.

### The settings

The component of its settings is `ideatek_rendering_vulkan:Settings`.
The defaults that cost time are provisional. A bench on the reference hardware gives them, by the need Measured defaults of `docs/contributing/needs.md`.

These settings change in play:

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `lod_threshold` | 1 | Above 0 | The error on the screen, in pixels, under which a coarser level is good. |
| `occlusion` | `false` | `true` or `false` | The test of the instances behind other geometry. |
| `diffuse` | `lambert` | `lambert` or `burley` | The diffuse light of a surface. |
| `specular` | `ggx` | `ggx` or `none` | The specular light of a surface. `none` keeps only the diffuse light. |
| `shading` | `pixel` | `pixel` or `vertex` | Where the module computes the light: at each pixel, or at each vertex. |
| `stats` | `true` | `true` or `false` | The counts of a frame. |
| `shader_folder` | Empty | A path that the path rule of the files part allows, or empty | The folder of the shaders that replace the shaders of the module. |

These settings take effect at the load of the module:

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `world_precision` | `double_float` | `double_float` or `float` | The places on the GPU: 2 `float` values for each number, or 1. |
| `origin_distance` | 1000000000 | 1 to 1e15 | The distance, in m, from the origin of the module at which it moves. |
| `max_instances` | 65536 | 1 to 16777216 | The instances on the GPU at a time. |
| `max_lights` | 1024 | 1 to 65536 | The lights in a frame. |
| `cluster_x`, `cluster_y`, `cluster_z` | 16, 9, 24 | 1 to 128 | The cells of the view for the lights, across, up and in depth. |
| `lights_per_cell` | 256 | 1 to 65536 | The most lights in 1 cell. |
| `geometry_bytes` | 268435456 | 1048576 to 17179869184 | The memory of the GPU for the vertices and the indices. |
| `texture_bytes` | 536870912 | 1048576 to 17179869184 | The memory of the GPU for the textures. |
| `capture_bytes` | 67108864 | 0 to 1073741824, an even number | The bytes of the persistent arena for 2 captures. |

2 captures of 3840 x 2160 fit in the default of `capture_bytes`: 3840 x 2160 x 4 x 2 = 66355200, which is less than 67108864.
A capture of more than the half of `capture_bytes` gets `IDEATEK_CAPTURE_STATUS_TOO_BIG`.
An instance, a light or a resource beyond its setting is not drawn. *The messages* gives the line of each kind.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module builds pipelines on a thread of its own. |
| `reloadable` | `false` | The pipelines and the memory of the GPU are not in the storage of Bridge. |
| `spawns` | 64 | 1 entity for each request of a shader of `shader_folder`, and for the watch. |
| `changes` | 65536 | The captures and the counts of a frame, and the requests of the shaders. |
| `persistent` | 67174400 | At least `capture_bytes`, plus 65536 for the texts of the settings and the paths. |
| `scratch` | 1048576 | The lists of the changed entities of a round. |
| `log_ring` | 2048 | 64 lines, with 32 bytes for each line. |
| `spans` | 65536 | The cells of its types and the lists of its observers. |

- The module takes the grants of the render module: `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The grants*.
  It also takes the grants of a drawer of the GPU part, `docs/extern/platform_abstraction/ideatek_gpu.h.md` and `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`, *The grants*, and reads `ideatek:Frame`.
  With `shader_folder`, it also takes the grants of a game of the files part: `docs/extern/platform_abstraction/ideatek_files.h.md`.
- Its system `draw` runs in the last phase of the deployment, so it comes after `ideatek_gpu_vulkan:gpu:begin` of the first phase.
  A pair puts it before `ideatek_gpu_vulkan:gpu:present`.
- The module reads `ideatek:WorldTransform` after the barriers of the space module in the round, so a pair puts `draw` after the movers of the round.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `<setting>: <value> is outside <range>` | A setting is outside its range. The module is broken, and `draw` gives 1. |
| `no device: the gpu module has no device` | No entity has `ideatek:VulkanDevice` in the round. The module draws nothing in it. |
| `no device: vulkan refuses the objects of the module` | Vulkan refuses the pool, the command buffers or the read buffers. The module draws nothing until a new device. |
| `no image for the window: vulkan refuses it` | Vulkan refuses the image of the module at the size of the window. The module draws nothing until the size changes. |
| `<entity> has no Visibility, so it is not drawn` | An entity with a mesh has no `ideatek:Visibility`. The line comes once for each entity. |
| `<entity> is not drawn: <reason>` | The module cannot read a stream, a format or a number of joint sets of the mesh or the texture. The line comes once. |
| `the shader <name> keeps the shader of the module: <reason>` | A shader of `shader_folder` does not come back, or does not compile. |
| `no room for <kind> in round <round>: <count> are not drawn` | More instances, lights or resources than their setting. |
