# BM-IdeaTek-Gpu-Vulkan

The GPU module of IdeaTek on Vulkan. It gives the GPUs, the device and the window images to the drawers.

## Install

The release of `BM-IdeaTek-Gpu-Vulkan` has the tag `gpu_vulkan_module-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Gpu-Vulkan.zip` and `sha256.txt`. `BM-IdeaTek-Gpu-Vulkan.zip` has a folder for each target.
In `BM-IdeaTek-Gpu-Vulkan`, each folder holds the module `gpu_vulkan.dll`, `gpu_vulkan.so` or `gpu_vulkan.dylib`, and the notices of volk and of the headers of Vulkan.
Each folder of `BM-IdeaTek-Gpu-Vulkan.zip` also holds this readme and the licence.

The module needs no library beside it: volk loads the loader of Vulkan of the machine at start.
A machine with no loader of Vulkan, or with no Vulkan 1.4, gets no entity of `ideatek:Gpu`, and the module writes 1 line.
A project names the zip of `BM-IdeaTek-Gpu-Vulkan` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_gpu_vulkan:gpu@0.1.0`.
Its role is the GPU module: `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The roles*.
Its handles of Vulkan are `ideatek:VulkanDevice` and `ideatek:VulkanSwapchain`: `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`.
The component of its settings is `ideatek_gpu_vulkan:Settings`. It holds the settings of `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The settings of the GPU module*: `frames_in_flight`, `image_count` and `validation`. The module adds no setting of its own.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module takes an image and gives it back in the phases. |
| `reloadable` | `false` | The device of Vulkan and its queues are not in the storage of Bridge. |
| `spawns` | 64 | 1 entity for each adapter of the machine, and 1 for the device. |
| `changes` | 4096 | The module attaches its values in the rounds of a new device and of a new swapchain. |
| `persistent` | 512 | Room for the log lines. |
| `scratch` | 16384 | Room for the declarations, the list of the GPUs and the messages of the validation layer. |
| `log_ring` | 2048 | Bytes of the ring of the log, 32 for each line. The layer of validation writes many lines. |
| `spans` | 65536 | The room of the lists of the 3 observers. |

- The module takes the grants that the 2 headers of the GPU part give to the GPU module: `docs/extern/platform_abstraction/ideatek_gpu.h.md` and `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`, *The grants*.
- The deployment loads the module before the window module.
  The module reads `ideatek:WindowState` for the handles of the surface. So it starts before the window opens, and the request of the swapchain comes later.
- Its system `begin` runs in its phase `first`.
  It waits for the round `frame` minus `frames_in_flight`, takes the image of the round, and puts it in the layout of a drawer.
- Its system `present` runs in its phase `last`.
  It puts the image in the layout of the window, gives its work to the queue, and shows the image.
- A pair puts this module before each drawer in the phase of `begin`, and after each drawer in the phase of `present`.
  A pair also orders each 2 drawers by the order in which they use the queues.

### The messages

The log of the module holds these lines:

| Line | Cause |
| --- | --- |
| `no loader: <reason>` | The machine has no loader of Vulkan. The module loads, and no GPU gets an entity. |
| `no instance: <result>` | Vulkan refuses the instance. No GPU gets an entity. |
| `no validation layer` | `validation` is `true`, and the layer of Khronos is not on the machine. The module goes on with no layer. |
| `the gpu <entity> is not usable: <reason>` | The request names no adapter, or an adapter with no Vulkan 1.4 or with a lack of Roadmap 2022. The device gets the status `IDEATEK_GPU_STATUS_NONE`. |
| `a second request <entity> gets no device` | A second entity carries `ideatek:GpuRequest`. Only the first gets a device. |
| `the gpu lacks <features>` | The adapter does not have the features of the request that the line gives in hexadecimal. They stay off. |
| `no device: <result>` | Vulkan refuses the device. The device gets the status `IDEATEK_GPU_STATUS_NONE`. |
| `no surface: <result>` | The handles of the window have a kind with no surface, or Vulkan refuses the surface. The next round tries again. |
| `no swapchain: <result>` | Vulkan refuses the swapchain. The window gets the status `IDEATEK_SWAPCHAIN_STATUS_SKIPPED`, and the next round tries again. |
| `the present mode <mode> is not there: vsync` | The window cannot use `present_mode` of the request. The swapchain uses `IDEATEK_PRESENT_MODE_VSYNC`. |
| `no hdr on the window: srgb` | `hdr` of the request is `true`, and the surface has no format of HDR. The swapchain uses `IDEATEK_COLOR_SPACE_SRGB`. |
| `the device is lost` | A call of Vulkan loses the device. The status is `IDEATEK_GPU_STATUS_LOST` for 1 round, then a new device comes. |

With `validation`, the layer and the loader of Vulkan add their own lines to the log, as warnings.
A setting outside its range gets its line of `readme.md`, *The contracts*.
