# src/platform_abstraction/gpu_vulkan_module/gpu_images.c

This file takes the images of the swapchain, and destroys the swapchain and the surface.

`take_images` takes at most `GPU_IMAGES` images, and keeps the size of the swapchain.
For each image, `keep_image` makes 1 view of the whole image and the semaphore `done` of the image: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_round.c.md`, *The semaphores*.
`drop_swapchain` waits for the device, then destroys the views, their semaphores and the swapchain. With no device, it does nothing.
`drop_surface` destroys the swapchain and the surface, and clears `no_present`: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_surface.c.md`, *The present*.

## Functions

| Function | Result |
| --- | --- |
| `keep_image` | Makes the view and the semaphore of 1 image. |
| `take_images` | Takes the images of the swapchain, and keeps their views and semaphores. |
| `drop_swapchain` | Destroys the views, their semaphores and the swapchain. |
| `drop_surface` | Destroys the swapchain and the surface. |
