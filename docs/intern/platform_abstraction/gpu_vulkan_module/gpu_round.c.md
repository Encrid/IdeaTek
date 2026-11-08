# src/platform_abstraction/gpu_vulkan_module/gpu_round.c

This file takes an image of the window in the phase `first`, and shows it in the phase `last`.

## The take

`begin` settles the module: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_settle.c.md`. A broken module gives 1.
A swapchain and a window that can draw take an image. A window with no image gets the status `IDEATEK_SWAPCHAIN_STATUS_SKIPPED`.

`take_image` does these steps:

1. `frame` grows by 1.
2. It waits on the timeline for the round `frame` minus `frames_in_flight`, so the objects of the slot are free.
3. It takes the next image. The acquire signals the semaphore `taken` of the slot. A result below 0 skips the round: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_lost.c.md`, *The results*.
4. It moves the image to the layout `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL`. That work waits on `taken` of the slot. It attaches the values of the round with the status ready.

## The show

`present` moves the image to the layout `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`. That work signals the semaphore `done` of the image, and the timeline with `frame`.
Then it shows the image. The show waits on `done` of the image.

The 2 barriers use the command buffer of the slot of the round. `gpu_submit_t` describes each barrier: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_inside.h.md`, *The names*.
The result of each submit goes to `note_result`, like the result of the show.

## The semaphores

`taken` has 1 semaphore for each slot. The acquire of a slot signals it after the wait on the timeline, so the semaphore of the slot is free.
`done` has 1 semaphore for each image. The acquire of an image proves that its last show has waited on it, so the semaphore of the image is free: https://github.com/KhronosGroup/Vulkan-Docs/issues/1678.

## The branches

| Case | Result |
| --- | --- |
| The image is out of date. | The round is skipped, and the next round makes a new swapchain. |
| A round after `frames_in_flight` rounds. | The wait on the timeline for an older round. |
| A round with no image. | No show. |

## Functions

| Function | Result |
| --- | --- |
| `record_barrier` | Records 1 barrier of the image of the round, and gives its command buffer. |
| `submit_barrier` | Records and submits 1 barrier with its semaphores. |
| `take_image` | Takes the image of the round. |
| `begin` | Settles the module and takes the image of the round. |
| `present` | Moves the image to the layout of the window, and shows it. |
