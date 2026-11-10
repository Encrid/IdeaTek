# src/engine_subsystems/loader_gltf/gltf_images.c

This file says the faults of the images of a file in the first pass.

An image that the KTX2 loader cannot read gives 1 line. An image whose textures have 2 samplers gives 1 line, because only the first sampler reaches its load.

## Functions

| Function | Result |
| --- | --- |
| `image_of` | The image of a texture, also of `KHR_texture_basisu`. |
| `has_second_sampler` | `true` when 2 textures of the image have 2 samplers. |
| `say_images` | Says the lines of the images. |
