# src/platform_abstraction/gpu_vulkan_module/gpu_settle.c

This file brings the device, the surface and the swapchain to the state that the requests ask for, at the start of each round.

## The settle

`settle` does these steps:

1. The first round reads the settings: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_gpus.c.md`. A broken module stops here.
2. A drop that waits since the leave of the request counts 1 down, and destroys the device at 0.
3. A different request while a device lives first gives the status none for 1 round, with `release_device`. So each drawer frees its objects while the device lives.
4. Else a new request makes the device: the request names an adapter, or 0 for the default.
5. A device that was lost in the round before asks for a new device in the next round. The round of the loss was the round of the drawers.
6. A device and a window with handles make the surface, unless the queue cannot show on it: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_surface.c.md`, *The present*.
7. A stale swapchain goes.
8. A device, a surface, a request of the swapchain and a window that can draw make the swapchain.

The observer `swap` keeps the request of the swapchain of the window, and marks the swapchain as stale. A request on a different entity changes nothing.

`attach_round` attaches `ideatek:Swapchain` with the frame, the slot and the status of the round, and `ideatek:VulkanSwapchain` with the status ready.

## The branches

| Case | Result |
| --- | --- |
| A window with no handles. | No surface, and the status skipped. |
| A minimized window. | No new swapchain. |
| A request of the swapchain on a different entity. | No change. |

## Functions

| Function | Result |
| --- | --- |
| `settle_device` | Picks the adapter of the request, and makes the device. |
| `settle_window` | Makes the surface and the swapchain that are missing. |
| `settle` | Runs the steps of the start of a round. Gives `false` when the module is broken. |
| `attach_round` | Attaches the values of the round on the entity of the window. |
| `swap` | Keeps the request of the swapchain of the window. |
