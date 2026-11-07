# src/platform_abstraction/gpu_vulkan_module/gpu_device.c

This file makes the device for the first entity that carries `ideatek:GpuRequest`.

## The request

`on_request` runs when the game writes the request:

1. A second entity with the request gets `a second request <entity> gets no device`, and nothing more.
2. The same entity with the same GPU changes nothing.
3. The same entity with a different GPU drops its device and asks for a fresh one.
4. A GPU of 0 names the default. A GPU that is not usable, or that no entity holds, gives `the gpu <entity> is not usable: <reason>` and the status `IDEATEK_GPU_STATUS_NONE`.

## The making

`make_device` does these steps:

1. It finds the 3 families of queues. Graphics comes first. Then a family with compute and no graphics, then a family with transfer only. A missing family takes the graphics family.
2. It chains the feature structures of 1.1 to 1.4 and the extensions of Roadmap 2024 that the adapter has. Every feature of the chains is on.
The 2 structures of rays and of mesh shaders join the chain when the request asks them.
3. A feature that the request asks for and that the adapter lacks gives `the gpu lacks <feature>`, and stays off.
4. It makes the device with the extensions that the chains need, and the timeline semaphore.
5. It attaches `ideatek:GpuDevice` with the generation and the status `IDEATEK_GPU_STATUS_READY`.
It attaches `ideatek:VulkanDevice` with the handles and the 3 queues.

```pseudo
FUNCTION make_device(api : POINTER, entity : U32) RETURNS BOOLEAN
   CALL find_queues()
   r <- vkCreateDevice()
   IF r <> SUCCESS THEN
      CALL say("no device: <reason>")
      CALL attach_none()
      RETURN false
   ENDIF
   generation <- generation + 1
   CALL attach_device(api, entity)
   RETURN true
ENDFUNCTION
```

`generation` starts at 1 and grows at each new device.
A drawer that reads a new generation makes its objects again: `docs/extern/platform_abstraction/ideatek_gpu.h.md`, *The device*.

## The loss

A result of `VK_ERROR_DEVICE_LOST` from a call of the module sets the status to `IDEATEK_GPU_STATUS_LOST` and `lost` to `true`.
The next round destroys the device and makes it again, with the same request.
The loss path lives in this file: `lose_device` writes the line, waits for nothing, and marks the remake.

## The branches

| Case | Result |
| --- | --- |
| The first entity with the request. | A device comes. |
| A second entity with the request. | The line of the second request. |
| A GPU of 0. | The default adapter is used. |
| An adapter without the feature of rays. | The line of the feature, and the feature stays off. |
| Vulkan refuses the device. | The line of the device, and the status `NONE`. |
| A call loses the device. | The status `LOST` for 1 round, then a new device. |

## Functions

| Function | Result |
| --- | --- |
| `on_request` | Follows the entity of the request, and makes or remakes the device. |
| `make_device` | Makes the device, the queues and the semaphore, and attaches the values. |
| `find_queues` | Fills the 3 families of queues from the adapter. |
| `device_features` | Builds the chain of features for the request. |
| `lose_device` | Marks the loss, writes the line, and asks the remake. |
