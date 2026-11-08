# src/platform_abstraction/gpu_vulkan_module/gpu_inside.h

This header gives the state of the module between 2 calls, the slots of its types, and the names that its sources share.

## The state

1 global variable `gpu_module` holds the state, because no argument that Bridge passes holds it.
The rule G4 of the style makes an exception for it, as for the window module and the input module.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A setting outside its range breaks the module. |
| `stale`, `lost`, `remake` | The swapchain must be made again; the device was lost in this round; the device must be made again. |
| `instance`, `messenger` | The handles of the instance and of its messenger. |
| `gpus`, `gpu_count`, `default_gpu`, `chosen` | The adapters with their entities, their count, the default, and the adapter of the device. |
| `device`, `physical`, `queues`, `families`, `timeline` | The device, its adapter, its 3 queues, their families, and the semaphore of the type timeline. |
| `taken`, `done` | The semaphore of each slot that the acquire signals, and the semaphore of each image that the last barrier signals. |
| `pool`, `buffers` | The pool of the module, and 1 command buffer for each frame. |
| `request`, `asked`, `generation` | The entity of the request, its last value, and the number of the device. |
| `window`, `surface_kind`, `native_display`, `native_window` | The entity of the window and its handles. |
| `pixel_width`, `pixel_height`, `drawable` | The pixel size, and `true` when a round can draw into the window. |
| `window_hdr`, `no_present` | `hdr` of the window, and `true` when the graphics queue cannot show on the surface of the window. |
| `swap_asked`, `swap_request`, `surface`, `swapchain` | The request of the swapchain, the surface and the swapchain. |
| `swap_images`, `views`, `image_count`, `format`, `color_space`, `swap_value` | The images of the swapchain, their views, and the value of `ideatek:Swapchain`. |
| `frame`, `image_index`, `got_image` | The round of the device, the image of the round, and whether the round took one. |

The array `gpus` has 1 more slot than `GPU_GPUS`. That slot stays empty, so `chosen` can name no adapter with no test.

## The names

The constants are `GPU_GPUS` (16 adapters), `GPU_IMAGES` (4 images) and `GPU_FRAMES` (3 frames). `GPU_MAX_LOG` is 256 bytes of a line of the layer, and `GPU_PATH` is 4096 bytes of a path.

The settings are `gpu_settings_t`: `loader` is a text, the 2 counts and `validation`.
The slots of the types start at 11, after the 11 enums of the declarations.
`GPU_TYPE` declares a type of the contract, and `GPU_F10` to `GPU_F13` give the place of a feature in the chain of features.
`gpu_surface_info_t` is the common start of the 4 infos of a surface, and `gpu_make_surface_t` is the type of their calls.
`gpu_submit_t` gives a barrier its 2 layouts, the semaphore to wait on, the semaphore to signal and the value of the timeline.
