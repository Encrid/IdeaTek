# src/platform_abstraction/gpu_vulkan_module/gpu_main.c

This file loads the module and follows the entity of the request.

## The load

`init` declares the types and the systems. It calls no Vulkan, because Bridge writes the settings after `init`: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_load.
The first round reads the settings and loads Vulkan: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_gpus.c.md`.
`reload` gives 0, because the manifest says that the module is not reloadable.

## The request

The observer `request` sees each write and each detach of `ideatek:GpuRequest`:

1. A detach destroys the device, and detaches `ideatek:GpuDevice` and `ideatek:VulkanDevice` from the entity.
2. A request of a second entity writes `a second request <entity> gets no device`, and changes nothing.
3. Else the module keeps the entity and the value. A new entity or a different value asks for a new device.

The same value again keeps the device. A different GPU or different features make a new device in the next round.

## The branches

| Case | Result |
| --- | --- |
| The first request. | A device in the next round. |
| The same request again. | The device stays. |
| A request with a different value. | A new device, with the next generation. |
| A second entity with a request. | The line of the second request. |
| A detach of the request. | The device goes, and its values leave. |

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares the types and the systems. |
| `leave_request` | Destroys the device and detaches its values. |
| `take_request` | Keeps the request of the first entity, and asks for a new device when the value changes. |
| `request` | Follows the entity of the request. |
| `reload` | Gives 0. |
