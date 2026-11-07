# src/platform_abstraction/gpu_vulkan_module/gpu_declare.c

This file declares the types and the calls of the module to Bridge.

## The declarations

The module declares the 9 common types of the contract, the 4 structures of the GPU part and the 2 structures of Vulkan.
The order of the declarations is in `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The declarations*.
The enums and the bitmask come before the structures, as the contract tells.
The declarations are data: `BRIDGE_NATIVE_DECLARE` of each type, then the module with its 2 systems and its 3 observers.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `begin` |
| System | `last` | `present` |
| Observer | write | `request` on `ideatek:GpuRequest` |
| Observer | write | `swap` on `ideatek:SwapchainRequest` |
| Observer | write | `window` on `ideatek:WindowState` |

The observers see only writes. When the entity of the request goes away, `on_destroy` ends the run of the module.
The module declares no helper type and no tag beside `ideatek:DefaultGpu`.

## The branches

| Case | Result |
| --- | --- |
| A detach of the entity of the request. | The device is destroyed in the next round. |

The file holds 1 function `declare` that Bridge calls at the load.
