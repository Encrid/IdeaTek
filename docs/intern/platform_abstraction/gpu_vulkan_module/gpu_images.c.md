# src/platform_abstraction/gpu_vulkan_module/gpu_images.c

This file takes the images of the swapchain and destroys the swapchain.

`take_images` takes at most `GPU_IMAGES` images, makes 1 view of the whole image for each, and keeps the size of the swapchain.
`drop_swapchain` waits for the device, then destroys the views and the swapchain. With no device, it does nothing.

## Functions

| Function | Result |
| --- | --- |
| `take_images` | Takes the images of the swapchain, and makes their views. |
| `drop_swapchain` | Destroys the views and the swapchain. |
