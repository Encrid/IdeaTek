# src/platform_abstraction/gpu_vulkan_module/gpu_swapchain.c

This file makes the swapchain of the window.

## The making

`make_swapchain` does these steps:

1. It reads the capacities of the surface. `image_count` goes into the range of the surface. A most of 0 means no most.
2. It picks the format: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_formats.c.md`. A request of HDR with no format of HDR writes `no hdr on the window: srgb`.
3. It picks the present mode. A mode with no place in the lists of the surface writes the line of the present mode.
4. It makes the swapchain. A refusal writes `no swapchain: <result>`, and the next round tries again.
5. It takes the images and makes their views: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_images.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A request of HDR, and no such format in the list of the surface. | The line of HDR, and sRGB. |
| A present mode outside the enum. | The line of the present mode, and vsync. |
| The swapchain is refused. | The line of the swapchain, and a swapchain in the next round. |

## Functions

| Function | Result |
| --- | --- |
| `choose_format` | Puts the format and the color space into the info. |
| `choose_present` | Puts the present mode into the info. |
| `apply_caps` | Puts the count of the images, the size and the transform into the info. |
| `make_swapchain` | Makes the swapchain and its views. Gives `false` with the line of the swapchain. |
