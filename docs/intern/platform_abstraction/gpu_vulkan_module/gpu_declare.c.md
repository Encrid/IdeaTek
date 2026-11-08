# src/platform_abstraction/gpu_vulkan_module/gpu_declare.c

This file declares the types and the calls of the module to Bridge.

## The declarations

The module declares the 11 enums and the bitmask first, then the 9 structures and the tag, then its settings.
The enums include `ideatek_window_mode_t` and `ideatek_surface_t`, because `ideatek:WindowState` names them.
The order of the types of the contract is in `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The declarations*.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `begin` |
| System | `last` | `present` |
| Observer | write and detach | `request` on `ideatek:GpuRequest` |
| Observer | write | `swap` on `ideatek:SwapchainRequest` |
| Observer | write and detach | `window` on `ideatek:WindowState` |

The 2 systems state the 6 types that the module writes. The observer `request` states `ideatek:GpuDevice` and `ideatek:VulkanDevice`, because it detaches them.

## Functions

| Function | Result |
| --- | --- |
| `declare_enums` | Declares the enums and the bitmask. |
| `declare_types` | Declares the structures, the tag and the settings. |
| `add_runs` | Declares the 2 systems and the 3 observers. |
