# BC-IdeaTek-Gpu

The contract of the GPU: the GPUs of the machine, 1 device, the swapchain and the handles of Vulkan.

## Install

The release of `BC-IdeaTek-Gpu` has the tag `gpu-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Gpu.zip` and `sha256.txt`. `BC-IdeaTek-Gpu.zip` has a folder for each target.
In `BC-IdeaTek-Gpu`, each folder holds `ideatek_gpu.h` and `ideatek_gpu_vulkan.h` in `include`.
Each folder of `BC-IdeaTek-Gpu.zip` also holds this readme and the licence.

`BC-IdeaTek-Gpu` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Gpu` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module includes the headers of `BC-IdeaTek-Gpu` that it needs:

| Header | Document |
| --- | --- |
| `ideatek_gpu.h` | `docs/extern/platform_abstraction/ideatek_gpu.h.md` |
| `ideatek_gpu_vulkan.h` | `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md` |

The parts in the `uses` of `BC-IdeaTek-Gpu` are `BC-IdeaTek-World`. Their readmes give their headers.
