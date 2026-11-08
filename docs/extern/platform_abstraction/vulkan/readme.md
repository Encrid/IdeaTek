# BL-IdeaTek-Vulkan

The headers of Vulkan 1.4.363 and the loader of calls volk 1.4.350 for the modules of IdeaTek that use Vulkan.

## Install

The release of `BL-IdeaTek-Vulkan` has the tag `vulkan-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-Vulkan.zip` and `sha256.txt`. `BL-IdeaTek-Vulkan.zip` has a folder for each target.
For each target, the zip of `vulkan` holds the static archive `volk.a` and the notices. The headers of Vulkan are in `include/vulkan` and `include/vk_video`, and `volk.h` is in `include`.

A project names the zip of `BL-IdeaTek-Vulkan` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `volk.h`, which includes the headers of Vulkan with `VK_NO_PROTOTYPES`, and links `volk.a`.
Each call of Vulkan is then a pointer that volk fills: https://github.com/zeux/volk/blob/1.4.350/README.md.

- `volkInitialize` loads the loader of Vulkan of the system: `vulkan-1.dll`, `libvulkan.so.1` or `libvulkan.1.dylib`. It fails when the system has none.
  On macOS it also looks for `/usr/local/lib/libvulkan.dylib`, where the Vulkan SDK puts the loader. It does not look in the folder of Homebrew on arm64.
- `volkInitializeCustom` takes a `vkGetInstanceProcAddr` that the module loaded itself, for a loader at a path of its own.
- `volkLoadInstance` fills the calls of an instance, and `volkLoadDevice` the calls of a device.

The test program `vulkan_probe` loads the loader and prints each GPU of the machine. It prints the features of the 2 milestones of Khronos that the GPU lacks.
With the environment variable `VULKAN_LOADER`, it loads the loader at that path.

`BL-IdeaTek-Vulkan` names no other part in `uses`.
