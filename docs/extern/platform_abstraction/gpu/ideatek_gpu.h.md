# include/platform_abstraction/gpu/ideatek_gpu.h

`include/platform_abstraction/gpu/ideatek_gpu.h` gives the types of the GPU part: the GPUs of the machine, 1 device and the swapchain of the window.
`readme.md`, *The contracts*, tells the rules of each contract.
The types of this header hold no value of a graphics API. A header of each API holds its handles: `docs/extern/platform_abstraction/gpu/ideatek_vulkan.h.md`.
`ideatek:GpuRequest` and `ideatek:GpuDevice` name entities of `include/platform_abstraction/world/ideatek_world.h`, so the GPU header includes it.

## The roles

- The game writes `ideatek:GpuRequest` and `ideatek:SwapchainRequest`.
- The GPU module writes `ideatek:Gpu`, `ideatek:DefaultGpu`, `ideatek:GpuDevice` and `ideatek:Swapchain`, and the types of its API. A deployment has one GPU module.
- A drawer reads all the types of the GPU part, and draws with the device. A renderer and the UI are drawers.

Out: a frame rate limit, a render scale, more than 1 queue, more than 1 device, and a GPU in a module of wasm.
Comes back: more queues, when a measure on a target shows that a drawer waits for the queue.

## The GPUs

At `init`, the GPU module spawns an entity for each GPU of the machine, and attaches `ideatek:Gpu` to it.
A GPU is usable when it has the features that the API of the GPU module needs.
The GPU module attaches the tag `ideatek:DefaultGpu` to 1 usable GPU. It takes a discrete GPU before the other kinds, and then the GPU with the most memory.
If no GPU is usable, no entity has `ideatek:DefaultGpu`.

## The device

The game spawns an entity and attaches `ideatek:GpuRequest` to it. `gpu` is a GPU entity, or 0 for the default GPU.
The GPU module makes the device for the first entity with `ideatek:GpuRequest`, and attaches `ideatek:GpuDevice` to it.
For each other entity, the GPU module writes 1 line in the log, and makes no device.
If the GPU is not usable, the status is `IDEATEK_GPU_STATUS_NONE`, and the log gets 1 line.

When the game attaches `ideatek:GpuRequest` again with a different GPU, the GPU module destroys the device and makes a new one.
When the device is lost, the status is `IDEATEK_GPU_STATUS_LOST` for 1 round. Then the GPU module makes a new device.
After each new device, `generation` grows by 1. A drawer then makes all its objects of the GPU again.
While the status is not `IDEATEK_GPU_STATUS_READY`, a drawer draws nothing.

## The swapchain

The game attaches `ideatek:SwapchainRequest` to the entity of the window.
When the window has `ideatek:WindowState` and the device is ready, the GPU module makes the swapchain.
It attaches `ideatek:Swapchain` to the entity of the window. It makes the swapchain again when the size of the window changes.

The GPU module uses `present_mode` if the window can use it. Else it uses `IDEATEK_PRESENT_MODE_VSYNC`, which each window can use.
With `hdr`, the GPU module uses a color space of HDR, if the `hdr` of `ideatek:WindowState` is `true` and the surface has such a format.
Else the color space is `IDEATEK_COLOR_SPACE_SRGB`.

## The rounds

The GPU module counts the rounds of the device from 1. `frame` is the number of the round.
`slot` is `frame` modulo `frames_in_flight`. A drawer keeps its objects of each frame in `frames_in_flight` sets, and uses the set of `slot`.
Before the GPU module takes an image in a round, it waits until the GPU finished the round `frame` minus `frames_in_flight`.
So the objects of the set of `slot` are free when the round starts.

At the start of a round, in the first phase, the GPU module shows the image of the round before, and takes the next image.
Each drawer then draws into the image, in the order of the deployment.
In the last phase, the GPU module gives the image to the window.
When the window has no image in a round, the status is `IDEATEK_SWAPCHAIN_STATUS_SKIPPED`, and no drawer draws to the window.
A minimized window and a window of size 0 have no image.

All the drawers and the GPU module use 1 queue. No 2 modules give work to the queue at the same time.
So a pair of the deployment puts the GPU module before each drawer in the first phase, and after each drawer in the last phase.
A pair also orders each 2 drawers of a phase, and that order is the order of their work on the queue.

## The settings of the GPU module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `frames_in_flight` | 2 | 2 to 3 | The rounds that the GPU can work on at the same time. |
| `image_count` | 3 | 2 to 4 | The images of the swapchain. |

## ideatek:Gpu

`ideatek:Gpu` gives a GPU of the machine.
`ideatek_gpu_t` takes 280 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `char[256]` | 0 | The name of the GPU in UTF-8, with a NUL byte at the end. |
| `memory_bytes` | `uint64_t` | 256 | The bytes of the memory of the GPU. For a GPU that shares the memory of the machine, the bytes that the GPU can use. |
| `vendor_id` | `uint32_t` | 264 | The PCI number of the maker of the GPU. |
| `device_id` | `uint32_t` | 268 | The number of the model of the GPU, from its maker. |
| `kind` | `ideatek_gpu_kind_t` | 272 | The kind of the GPU. |
| `ray_query` | `bool` | 273 | `true` if a shader can trace rays on the GPU. |
| `usable` | `bool` | 274 | `true` if the GPU module can make a device on the GPU. |

Bytes 275 to 279 hold no field.
The 256 bytes of `name` are `VK_MAX_PHYSICAL_DEVICE_NAME_SIZE` of Vulkan, so each name of a GPU fits.

### ideatek:ideatek_gpu_kind_t

`ideatek_gpu_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The kind |
| --- | --- | --- |
| `IDEATEK_GPU_KIND_INTEGRATED` | 0 | A GPU in the processor, which shares the memory of the machine. |
| `IDEATEK_GPU_KIND_DISCRETE` | 1 | A GPU on its own card or chip, with its own memory. |
| `IDEATEK_GPU_KIND_VIRTUAL` | 2 | A GPU of a virtual machine. |
| `IDEATEK_GPU_KIND_CPU` | 3 | A GPU that the processor runs as software. |
| `IDEATEK_GPU_KIND_OTHER` | 4 | A GPU of a different kind. |

## ideatek:DefaultGpu

`ideatek:DefaultGpu` is a tag. It has no bytes, and no C type.

## ideatek:GpuRequest

`ideatek:GpuRequest` asks for the device.
`ideatek_gpu_request_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gpu` | `ideatek_entity_t` | 0 | The GPU entity of the device, or 0 for the entity with `ideatek:DefaultGpu`. |

## ideatek:GpuDevice

`ideatek:GpuDevice` gives the state of the device.
`ideatek_gpu_device_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gpu` | `ideatek_entity_t` | 0 | The GPU entity of the device. |
| `generation` | `uint32_t` | 8 | The number of the device. The first device is 1. |
| `api` | `ideatek_gpu_api_t` | 12 | The graphics API of the device. |
| `status` | `ideatek_gpu_status_t` | 13 | The status of the device. |

Bytes 14 and 15 hold no field.

### ideatek:ideatek_gpu_api_t

`ideatek_gpu_api_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The API | The header of its handles |
| --- | --- | --- | --- |
| `IDEATEK_GPU_API_VULKAN` | 0 | Vulkan | `include/platform_abstraction/gpu/ideatek_vulkan.h` |

### ideatek:ideatek_gpu_status_t

`ideatek_gpu_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_GPU_STATUS_NONE` | 0 | The device is not there. |
| `IDEATEK_GPU_STATUS_READY` | 1 | A drawer can use the device. |
| `IDEATEK_GPU_STATUS_LOST` | 2 | The device is lost. In the next round, the GPU module makes a new device. |

## ideatek:SwapchainRequest

`ideatek:SwapchainRequest` asks for the swapchain of the window.
`ideatek_swapchain_request_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `present_mode` | `ideatek_present_mode_t` | 0 | The present mode that the game wants. |
| `hdr` | `bool` | 1 | `true` if the game wants a color space of HDR. |

### ideatek:ideatek_present_mode_t

`ideatek_present_mode_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | When the window shows an image |
| --- | --- | --- |
| `IDEATEK_PRESENT_MODE_VSYNC` | 0 | At each refresh of the display. An image waits for the refresh. |
| `IDEATEK_PRESENT_MODE_ADAPTIVE` | 1 | At each refresh. A late image shows at once, with a tear. |
| `IDEATEK_PRESENT_MODE_MAILBOX` | 2 | At each refresh, the newest image. An older image is dropped. |
| `IDEATEK_PRESENT_MODE_IMMEDIATE` | 3 | At once, with a tear. |

## ideatek:Swapchain

`ideatek:Swapchain` gives the swapchain of the window in the round.
`ideatek_swapchain_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `frame` | `uint64_t` | 0 | The number of the round: *The rounds*. |
| `width` | `uint32_t` | 8 | The width of the images, in pixels. |
| `height` | `uint32_t` | 12 | The height of the images, in pixels. |
| `slot` | `uint32_t` | 16 | The set of the objects of the round: *The rounds*. |
| `format` | `ideatek_image_format_t` | 20 | The format of the images. |
| `color_space` | `ideatek_color_space_t` | 21 | The color space of the images. |
| `present_mode` | `ideatek_present_mode_t` | 22 | The present mode that the GPU module uses. |
| `status` | `ideatek_swapchain_status_t` | 23 | The status in the round. |

### ideatek:ideatek_image_format_t

`ideatek_image_format_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The format |
| --- | --- | --- |
| `IDEATEK_IMAGE_FORMAT_BGRA8_SRGB` | 0 | 8 bits for blue, green, red and alpha, with the sRGB curve. |
| `IDEATEK_IMAGE_FORMAT_RGBA8_SRGB` | 1 | 8 bits for red, green, blue and alpha, with the sRGB curve. |
| `IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM` | 2 | 10 bits for red, green and blue, and 2 bits for alpha. |
| `IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM` | 3 | 10 bits for blue, green and red, and 2 bits for alpha. |
| `IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT` | 4 | A 16 bit float for red, green, blue and alpha. |

### ideatek:ideatek_color_space_t

`ideatek_color_space_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The color space |
| --- | --- | --- |
| `IDEATEK_COLOR_SPACE_SRGB` | 0 | sRGB. |
| `IDEATEK_COLOR_SPACE_HDR10` | 1 | The primaries of BT.2020 and the curve of ST 2084. |
| `IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR` | 2 | The primaries of sRGB, linear, and values out of 0 to 1. |

### ideatek:ideatek_swapchain_status_t

`ideatek_swapchain_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_SWAPCHAIN_STATUS_READY` | 0 | The round has an image of the window. |
| `IDEATEK_SWAPCHAIN_STATUS_SKIPPED` | 1 | The round has no image of the window. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_gpu_kind_t` | `ideatek_gpu_kind_t` | `ideatek_gpu_kind_t_desc` | `IDEATEK_GPU_KIND_VALUES` |
| `ideatek:ideatek_gpu_api_t` | `ideatek_gpu_api_t` | `ideatek_gpu_api_t_desc` | `IDEATEK_GPU_API_VALUES` |
| `ideatek:ideatek_gpu_status_t` | `ideatek_gpu_status_t` | `ideatek_gpu_status_t_desc` | `IDEATEK_GPU_STATUS_VALUES` |
| `ideatek:ideatek_present_mode_t` | `ideatek_present_mode_t` | `ideatek_present_mode_t_desc` | `IDEATEK_PRESENT_MODE_VALUES` |
| `ideatek:ideatek_image_format_t` | `ideatek_image_format_t` | `ideatek_image_format_t_desc` | `IDEATEK_IMAGE_FORMAT_VALUES` |
| `ideatek:ideatek_color_space_t` | `ideatek_color_space_t` | `ideatek_color_space_t_desc` | `IDEATEK_COLOR_SPACE_VALUES` |
| `ideatek:ideatek_swapchain_status_t` | `ideatek_swapchain_status_t` | `ideatek_swapchain_status_t_desc` | `IDEATEK_SWAPCHAIN_STATUS_VALUES` |
| `ideatek:Gpu` | `ideatek_gpu_t` | `ideatek_gpu_t_desc` | `IDEATEK_GPU_FIELDS` |
| `ideatek:GpuRequest` | `ideatek_gpu_request_t` | `ideatek_gpu_request_t_desc` | `IDEATEK_GPU_REQUEST_FIELDS` |
| `ideatek:GpuDevice` | `ideatek_gpu_device_t` | `ideatek_gpu_device_t_desc` | `IDEATEK_GPU_DEVICE_FIELDS` |
| `ideatek:SwapchainRequest` | `ideatek_swapchain_request_t` | `ideatek_swapchain_request_t_desc` | `IDEATEK_SWAPCHAIN_REQUEST_FIELDS` |
| `ideatek:Swapchain` | `ideatek_swapchain_t` | `ideatek_swapchain_t_desc` | `IDEATEK_SWAPCHAIN_FIELDS` |

```text
{ IDEATEK_GPU_KIND_INTEGRATED = 0, IDEATEK_GPU_KIND_DISCRETE = 1, IDEATEK_GPU_KIND_VIRTUAL = 2, IDEATEK_GPU_KIND_CPU = 3, IDEATEK_GPU_KIND_OTHER = 4 }
{ IDEATEK_GPU_API_VULKAN = 0 }
{ IDEATEK_GPU_STATUS_NONE = 0, IDEATEK_GPU_STATUS_READY = 1, IDEATEK_GPU_STATUS_LOST = 2 }
{ IDEATEK_PRESENT_MODE_VSYNC = 0, IDEATEK_PRESENT_MODE_ADAPTIVE = 1, IDEATEK_PRESENT_MODE_MAILBOX = 2, IDEATEK_PRESENT_MODE_IMMEDIATE = 3 }
{ IDEATEK_IMAGE_FORMAT_BGRA8_SRGB = 0, IDEATEK_IMAGE_FORMAT_RGBA8_SRGB = 1, IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM = 2, IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM = 3, IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT = 4 }
{ IDEATEK_COLOR_SPACE_SRGB = 0, IDEATEK_COLOR_SPACE_HDR10 = 1, IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR = 2 }
{ IDEATEK_SWAPCHAIN_STATUS_READY = 0, IDEATEK_SWAPCHAIN_STATUS_SKIPPED = 1 }
{ char name[256]; uint64_t memory_bytes; uint32_t vendor_id; uint32_t device_id; ideatek_gpu_kind_t kind; bool ray_query; bool usable; }
{ ideatek_entity_t gpu; }
{ ideatek_entity_t gpu; uint32_t generation; ideatek_gpu_api_t api; ideatek_gpu_status_t status; }
{ ideatek_present_mode_t present_mode; bool hdr; }
{ uint64_t frame; uint32_t width; uint32_t height; uint32_t slot; ideatek_image_format_t format; ideatek_color_space_t color_space; ideatek_present_mode_t present_mode; ideatek_swapchain_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gpu_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gpu_api_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gpu_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_present_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_image_format_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_color_space_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_swapchain_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Gpu` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DefaultGpu` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GpuRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GpuDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SwapchainRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Swapchain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

The seven enums and `ideatek:ideatek_entity_t` come before the structures in the declarations of a module.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:GpuRequest` and `ideatek:SwapchainRequest` | `ideatek:ideatek_entity_t`, the seven enums and the four types of the GPU module |
| The GPU module | `ideatek:Gpu`, `ideatek:DefaultGpu`, `ideatek:GpuDevice`, `ideatek:Swapchain` and the types of its API | `ideatek:ideatek_entity_t`, the seven enums, the two types of the game and `ideatek:WindowState` |
| A drawer | No type. | `ideatek:ideatek_entity_t`, the seven enums, the six types of the GPU part and the types of the API |
