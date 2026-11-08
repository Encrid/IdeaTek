# src/platform_abstraction/gpu_vulkan_module/gpu_adapters.c

This file reads the adapters of the machine, makes their entities, and picks the default.

## The adapters

`list_gpus` asks Vulkan for at most `GPU_GPUS` adapters in 1 call. A refused call gives no adapter.
`add_gpu` reads each adapter into a value of `ideatek:Gpu`, and spawns its entity:

| Field | Source |
| --- | --- |
| `name` | `deviceName`, cut at 255 bytes. |
| `memory_bytes` | The sum of the heaps with `VK_MEMORY_HEAP_DEVICE_LOCAL_BIT`. |
| `kind` | `deviceType`, in the order of the contract. |
| `features` | `VK_KHR_ray_query`, `VK_KHR_ray_tracing_pipeline`, `VK_EXT_mesh_shader` and `drawIndirectCount`. |
| `usable` | Vulkan 1.4 and no lack of Roadmap 2022: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_offers.c.md`. |

## The default

Each adapter gets a score. An adapter that is not usable has 0. A discrete adapter is above each other kind, and more memory is above less.
The adapter with the highest score is the default, and the first of Vulkan keeps a tie. No usable adapter gives no default.
The pick is arithmetic, so it has no branch for 1 adapter or for many.

## The branches

| Case | Result |
| --- | --- |
| The list of the adapters is refused. | No adapter and no default. |
| At least 1 usable adapter. | `ideatek:DefaultGpu` on the adapter of the highest score. |

## Functions

| Function | Result |
| --- | --- |
| `features_of` | Gives the features of the contract that an adapter has. |
| `local_memory` | Gives the bytes of the memory of an adapter. |
| `read_gpu` | Gives the value of `ideatek:Gpu` of an adapter. |
| `add_gpu` | Spawns the entity of an adapter, and keeps the best score. |
| `list_gpus` | Spawns the entity of each adapter, and attaches the tag of the default. |
