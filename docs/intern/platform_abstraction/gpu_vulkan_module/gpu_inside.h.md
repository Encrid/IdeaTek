# src/platform_abstraction/gpu_vulkan_module/gpu_inside.h

This header gives the state of the module between 2 calls, and the names that its sources share.

## The state

1 global variable `gpu_module` holds the state, because no argument that Bridge passes holds it.
The rule G4 of the style makes an exception for it, as for the window module and the input module.

The fields hold the instance and its messenger, and the table of the adapters with their entities.
More fields hold the device with its queues and its semaphore. The window has its surface, its swapchain and the pixel size. The settings and the state of the round close the structure.
The name of each field is the name that the code uses.

| Field | Value |
| --- | --- |
| `gpus` and `gpu_entities` | The `VkPhysicalDevice` and the entity of each adapter, to `gpu_count`. |
| `default_gpu` | The index of the default adapter. |
| `device`, `queues`, `families`, `timeline` | The device, its 3 queues, their families, and the semaphore of the type timeline. |
| `request` | The entity that carries `ideatek:GpuRequest`. |
| `generation` | The number of the device, from 1. |
| `surface`, `swapchain`, `swap_width`, `swap_height` | The handles of the window images and their pixel size. |
| `frame`, `image_index`, `got_image` | The round of the device, the image of the round, and whether the round took one. |

`GPU_GPUS` is 16. A machine with more adapters gets 16 entities, in the order of Vulkan.

## The names

The constants are `GPU_GPUS`, `GPU_IMAGES` (the most images of a swapchain, 4) and `GPU_MAX_LOG` (256, the bytes of 1 line of the layer of validation).
The functions of the sources are in the table of each document of this folder.
A drawer records its own work. The module owns a pool with 1 command buffer of each frame for the 2 barriers of a round.
No drawer other than this module moves an image to the layout that ends the round.
