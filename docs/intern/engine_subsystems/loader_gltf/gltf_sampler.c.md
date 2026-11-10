# src/engine_subsystems/loader_gltf/gltf_sampler.c

This file gives the settings of a dependency load of an image, from the sampler of its first texture.

The settings are the lines of `docs/usage/engine_subsystems/loader_ktx2.md`, *The sampler*. A filter of 0 takes the linear filters of glTF.
`is_external` gives `true` for a file of its own whose name ends with `ktx2`, or whose type is `image/ktx2`.

## Functions

| Function | Result |
| --- | --- |
| `filter_of` | The word of a filter of OpenGL, for the minifying, the magnifying or the mip filter. |
| `wrap_of` | The word of a wrap of OpenGL. |
| `sampler_of` | The sampler of the first texture of an image, or nothing. |
| `sampler_settings` | The settings text of an image. |
| `is_external` | `true` for an image of its own in KTX 2.0. |
