# src/platform_abstraction/gpu_vulkan_module/gpu_device.c

This file makes the device on the chosen adapter.

## The making

`make_device` does these steps:

1. The number of the device grows by 1.
2. An adapter that is not usable, or no adapter, writes `the gpu <entity> is not usable: <reason>` and attaches the status `IDEATEK_GPU_STATUS_NONE`.
3. A feature of the request that the adapter lacks writes `the gpu lacks <features>`, in hexadecimal. It stays off.
4. It makes the device with `VK_KHR_swapchain` and 1 queue of each family that differs. Each feature of Vulkan 1.0 to 1.3 that the adapter has turns on.
5. A refused device writes `no device: <result>`, and attaches the status `IDEATEK_GPU_STATUS_NONE`.
6. `make_sync` takes the queues and makes the objects of the rounds: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_sync.c.md`. The semaphores serve `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_round.c.md`, *The semaphores*.
   A refusal of one of them makes the module destroy the device and attach the status `IDEATEK_GPU_STATUS_NONE`.
7. It attaches the status `IDEATEK_GPU_STATUS_READY` with the features that are on, and `ideatek:VulkanDevice`.

The features of Roadmap 2024 turn on with the chain of Vulkan 1.0 to 1.3, when the adapter has them.

## The branches

| Case | Result |
| --- | --- |
| A request for an entity that is no adapter. | The line of the GPU, and the status none. |
| A request for a feature that the adapter lacks. | The line of the feature. |
| The device is refused. | The line of the device, and the status none. |
| A semaphore, the pool or the command buffers are refused. | The line of the device, no device, and the status none. |
| The device is made. | The status ready. |

## Functions

| Function | Result |
| --- | --- |
| `queue_infos` | Gives 1 info of a queue for each family that differs. |
| `open_device` | Makes the device and loads its calls. Gives `false` at a refusal. |
| `make_device` | Makes the device, and attaches its status. |
