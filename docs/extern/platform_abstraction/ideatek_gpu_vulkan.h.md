# include/platform_abstraction/ideatek_gpu_vulkan.h

`include/platform_abstraction/ideatek_gpu_vulkan.h` gives the handles of Vulkan for a GPU module that uses Vulkan.
`docs/extern/platform_abstraction/ideatek_gpu.h.md` tells the common rules.
The header includes no header of Vulkan. Each handle is a `uint64_t`, and a drawer casts it to the type of Vulkan.

## The handles

The GPU module attaches `ideatek:VulkanDevice` to the entity of `ideatek:GpuDevice`, when the status is `IDEATEK_GPU_STATUS_READY`.
It attaches `ideatek:VulkanSwapchain` to the entity of the window in each round with an image.
A drawer makes no device and no swapchain. It gives its work to the 3 queues, and destroys no handle of the GPU module.
A drawer opens no loader of Vulkan. It takes each function of Vulkan from `get_instance_proc_addr`, so the GPU module and each drawer use 1 loader.

## The image of a round

In the first phase, the GPU module puts the image in the layout `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL`.
Each drawer leaves the image in that layout.
The first use of the image by a drawer waits on the stage `VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT` in a barrier.
In the last phase, the GPU module puts the image in the layout `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`.
That work signals `timeline` with the value `frame`.

## The features

A GPU is usable when it has Vulkan 1.4 and each feature of the milestone Roadmap 2022 of Khronos.
The GPU module turns on each feature that Vulkan 1.4 and Roadmap 2022 require, so each drawer can use it.
Among them are descriptor indexing, multi-draw indirect, dynamic rendering, synchronization2, buffer device address and push descriptors.
Each feature of the milestone Roadmap 2024 that the GPU has also turns on.
A GPU can lack a feature of Roadmap 2024 and stay usable, so a drawer asks the device before it uses such a feature.

The 2 features of rays also turn on `VK_KHR_deferred_host_operations`, which `VK_KHR_acceleration_structure` needs.
With `validation`, the GPU module turns on the layer `VK_LAYER_KHRONOS_validation`. If the layer is not there, the log gets 1 line.

## The values of Vulkan

| Value of IdeaTek | Value of Vulkan |
| --- | --- |
| `IDEATEK_IMAGE_FORMAT_BGRA8_SRGB` | `VK_FORMAT_B8G8R8A8_SRGB` |
| `IDEATEK_IMAGE_FORMAT_RGBA8_SRGB` | `VK_FORMAT_R8G8B8A8_SRGB` |
| `IDEATEK_IMAGE_FORMAT_RGB10A2_UNORM` | `VK_FORMAT_A2B10G10R10_UNORM_PACK32` |
| `IDEATEK_IMAGE_FORMAT_BGR10A2_UNORM` | `VK_FORMAT_A2R10G10B10_UNORM_PACK32` |
| `IDEATEK_IMAGE_FORMAT_RGBA16_FLOAT` | `VK_FORMAT_R16G16B16A16_SFLOAT` |
| `IDEATEK_COLOR_SPACE_SRGB` | `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR` |
| `IDEATEK_COLOR_SPACE_HDR10` | `VK_COLOR_SPACE_HDR10_ST2084_EXT` |
| `IDEATEK_COLOR_SPACE_EXTENDED_SRGB_LINEAR` | `VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT` |
| `IDEATEK_PRESENT_MODE_VSYNC` | `VK_PRESENT_MODE_FIFO_KHR` |
| `IDEATEK_PRESENT_MODE_ADAPTIVE` | `VK_PRESENT_MODE_FIFO_RELAXED_KHR` |
| `IDEATEK_PRESENT_MODE_MAILBOX` | `VK_PRESENT_MODE_MAILBOX_KHR` |
| `IDEATEK_PRESENT_MODE_IMMEDIATE` | `VK_PRESENT_MODE_IMMEDIATE_KHR` |
| `IDEATEK_GPU_FEATURE_RAY_QUERY` | `VK_KHR_acceleration_structure` and `VK_KHR_ray_query` |
| `IDEATEK_GPU_FEATURE_RAY_TRACING_PIPELINE` | `VK_KHR_acceleration_structure` and `VK_KHR_ray_tracing_pipeline` |
| `IDEATEK_GPU_FEATURE_MESH_SHADER` | `VK_EXT_mesh_shader`, with `taskShader` and `meshShader` |
| `IDEATEK_GPU_FEATURE_DRAW_INDIRECT_COUNT` | `drawIndirectCount` of `VkPhysicalDeviceVulkan12Features` |

`kind` of `ideatek:Gpu` comes from the `VkPhysicalDeviceType` of the GPU, in the same order.

## ideatek:VulkanDevice

`ideatek:VulkanDevice` gives the handles of the device.
`ideatek_vulkan_device_t` takes 80 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `instance` | `uint64_t` | 0 | The `VkInstance`. |
| `physical_device` | `uint64_t` | 8 | The `VkPhysicalDevice` of the GPU. |
| `device` | `uint64_t` | 16 | The `VkDevice`. |
| `queue` | `uint64_t` | 24 | The `VkQueue` of graphics. It can do graphics, compute and transfer. |
| `compute_queue` | `uint64_t` | 32 | The `VkQueue` of async compute. It can do compute and transfer. |
| `transfer_queue` | `uint64_t` | 40 | The `VkQueue` of uploads, for the streaming. It can do transfer. |
| `timeline` | `uint64_t` | 48 | The `VkSemaphore` of the type timeline: *The image of a round*. |
| `get_instance_proc_addr` | `uint64_t` | 56 | The `vkGetInstanceProcAddr` of the loader that the GPU module opened. |
| `queue_family` | `uint32_t` | 64 | The index of the family of `queue`. |
| `compute_family` | `uint32_t` | 68 | The index of the family of `compute_queue`. |
| `transfer_family` | `uint32_t` | 72 | The index of the family of `transfer_queue`. |

Bytes 76 to 79 hold no field.
The GPU module takes `compute_queue` from a family with compute and no graphics.
It takes `transfer_queue` from a family with transfer and no graphics and no compute.
If the GPU has no such family, the handle is `queue`, and the index is `queue_family`.
A drawer that uses 1 image or 1 buffer on 2 families moves it from one family to the other with a barrier.

## ideatek:VulkanSwapchain

`ideatek:VulkanSwapchain` gives the image of the window in the round.
`ideatek_vulkan_swapchain_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `image` | `uint64_t` | 0 | The `VkImage` of the round. |
| `view` | `uint64_t` | 8 | A `VkImageView` of the whole image. |
| `format` | `int32_t` | 16 | The `VkFormat` of the image. |
| `color_space` | `int32_t` | 20 | The `VkColorSpaceKHR` of the image. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:VulkanDevice` | `ideatek_vulkan_device_t` | `ideatek_vulkan_device_t_desc` | `IDEATEK_VULKAN_DEVICE_FIELDS` |
| `ideatek:VulkanSwapchain` | `ideatek_vulkan_swapchain_t` | `ideatek_vulkan_swapchain_t_desc` | `IDEATEK_VULKAN_SWAPCHAIN_FIELDS` |

```text
{ uint64_t instance; uint64_t physical_device; uint64_t device; uint64_t queue; uint64_t compute_queue; uint64_t transfer_queue; uint64_t timeline; uint32_t queue_family; uint32_t compute_family; uint32_t transfer_family; }
{ uint64_t image; uint64_t view; int32_t format; int32_t color_space; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:VulkanDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VulkanSwapchain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The declarations*, tells the rules of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | `ideatek:VulkanDevice` and `ideatek:VulkanSwapchain` |
| The GPU module | `ideatek:VulkanDevice` and `ideatek:VulkanSwapchain` | No type. |
| A drawer | No type. | `ideatek:VulkanDevice` and `ideatek:VulkanSwapchain` |
