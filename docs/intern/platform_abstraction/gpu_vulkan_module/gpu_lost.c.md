# src/platform_abstraction/gpu_vulkan_module/gpu_lost.c

This file attaches the status of the device, ends the device, and watches the results of the calls of a round.

## The results

`note_result` reads the result of an acquire or of a show:

- `VK_ERROR_OUT_OF_DATE_KHR` and `VK_SUBOPTIMAL_KHR` mark the swapchain as stale. The next round makes it again.
- `VK_ERROR_DEVICE_LOST` writes `the device is lost` and attaches the status `IDEATEK_GPU_STATUS_LOST`.

The next round keeps the status lost, and the round after makes a new device with the same request.
Thus each drawer sees the status lost for 1 round: `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The device*. The loss also marks the device as released.
`release_device` attaches the status none, and marks the device as released, for the round before a new device.

## The end

`drop_device` waits for the device, destroys the swapchain, the pool, the semaphores and the device.
`shutdown` destroys the device, the surface, the messenger and the instance. It keeps the loader of Vulkan in the process.

## The branches

| Case | Result |
| --- | --- |
| A show gives a lost device. | The line of the loss, the status lost for 1 round, then a new device. |
| No device at the end. | `drop_device` does nothing. |

## Functions

| Function | Result |
| --- | --- |
| `attach_device` | Attaches `ideatek:GpuDevice`, and `ideatek:VulkanDevice` with the status ready. |
| `drop_device` | Destroys the device and its objects. |
| `note_result` | Marks the swapchain as stale, or the device as lost, from a result. |
| `release_device` | Attaches the status none for the round of the drawers. |
| `shutdown` | Destroys the objects of the module. |
