# src/platform_abstraction/gpu_vulkan_module/gpu_sync.c

This file makes the objects of a new device that let the rounds wait and record: the semaphores, the pool and the command buffers.

`make_sync` takes the 3 queues. It makes the semaphores with `make_semaphores`, then the pool and the command buffers with `make_pool`.
Each handle is null before its call, so a refused object leaves a null handle, and `drop_device` destroys only what was made.
A refusal writes `no device: <result>` with the first refused result: `first_result` keeps it through the chain of calls.
`make_pool` allocates no command buffer when the pool is refused.

## The branches

| Case | Result |
| --- | --- |
| A semaphore is refused. | The line of the device with its result, and `false`. |
| The pool is refused. | The line of the device, no command buffer, and `false`. |
| The command buffers are refused. | The line of the device, and `false`. |
| Each object is made. | `true`. |

## Functions

| Function | Result |
| --- | --- |
| `first_result` | Gives `kept` when it is a refusal, else `result`. So a chain of calls keeps its first refusal. |
| `make_semaphores` | Makes the timeline semaphore and the semaphore `taken` of each frame. Gives the first refused result, or `VK_SUCCESS`. |
| `make_pool` | Makes the pool and 1 command buffer for each frame. Gives the first refused result, or `VK_SUCCESS`. |
| `make_sync` | Takes the queues, and makes the semaphores, the pool and the command buffers. Gives `false` at a refusal. |
