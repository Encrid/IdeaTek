# src/engine_subsystems/rendering_vulkan/render_device.c

This file takes the device of the GPU module, and keeps the objects of Vulkan of the module on it.

## The device

`find_device` reads the entity that the observer `device` keeps: its `ideatek:GpuDevice` and its `ideatek:VulkanDevice`. A status other than ready counts as no device.
The GPU module attaches and detaches the 2 values together, so an `ideatek:VulkanDevice` always has its `ideatek:GpuDevice`.
With no device, the module draws nothing in the round. `tell_absence` writes the line of no device once, until a device comes.

Each new device comes after a round whose status is not ready: `docs/extern/platform_abstraction/ideatek_gpu.h.md`. In that round the module ends the copies of its captures as failed and frees each object. In the first ready round it makes its objects again:

1. `take_handles` gives `get_instance_proc_addr` of `ideatek:VulkanDevice` to `volkInitializeCustom`, so the module uses the loader of the GPU module. It keeps the handles.
   It fills the calls with `volkLoadInstanceOnly` and `volkLoadDevice`.
   The module links its own copy of volk, so the calls of the GPU module do not change.
2. `make_objects` makes 1 command pool on the family of `queue`, and 1 command buffer for each slot.
   It also makes the read buffers: `docs/intern/engine_subsystems/rendering_vulkan/render_memory.c.md`.

When Vulkan refuses an object, the module writes the line of the refusal, and keeps the device as refused. It tries again only after a round with no ready device.

## The slots

`slot` of `ideatek:Swapchain` names the set of objects of the round. The GPU module waits for the round `frame` minus `frames_in_flight` before it takes an image.
So the command buffer and the read buffer of the slot are free when `draw` runs, and the module makes no fence of its own.

## Functions

| Function | Result |
| --- | --- |
| `take_handles` | Keeps the handles of the device and fills the calls of Vulkan. |
| `make_objects` | Makes the pool, the command buffers and the read buffers. `false` when Vulkan refuses. |
| `tell_absence` | Writes the line of no device once, while no device is there. |
| `find_device` | `true` when the device of the round is ready, with the objects of the module. |
| `free_objects` | Waits until the device is idle, then frees each object of the module. |
