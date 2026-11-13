# src/platform_abstraction/gpu_vulkan_module/gpu_swapchain.c

This file makes the swapchain of the window.

## The making

`make_swapchain` does these steps:

1. It reads the capacities of the surface. `image_count` goes into the range of the surface. A most of 0 means no most.
2. It picks the format: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_formats.c.md`. HDR needs `hdr` of the request, `hdr` of the window and a format of HDR on the surface. A request of HDR that gets sRGB writes `no hdr: <reason>`. The reason names the display when `hdr` of the window is `false`, else the surface.
3. It picks the present mode. A mode with no place in the lists of the surface writes the line of the present mode.
4. It makes the swapchain, then takes the images and makes their views: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_images.c.md`.
5. A refusal of the swapchain, of a view or of a semaphore writes `no swapchain: <result>`. The module destroys what it made, and the next round tries again.

## The branches

| Case | Result |
| --- | --- |
| A request of HDR on a window with no HDR, or with no such format in the list of the surface. | The line of HDR with its reason, and sRGB. |
| A present mode outside the enum. | The line of the present mode, and vsync. |
| The swapchain is refused. | The line of the swapchain, and a swapchain in the next round. |
| A view or a semaphore of an image is refused. | The line of the swapchain, and a swapchain in the next round. |

## Functions

| Function | Result |
| --- | --- |
| `say_no_hdr` | Writes the line of HDR when the request asked for HDR and sRGB came. |
| `choose_format` | Puts the format and the color space into the info. |
| `choose_present` | Puts the present mode into the info. |
| `apply_caps` | Puts the count of the images, the size and the transform into the info. |
| `make_swapchain` | Makes the swapchain and its views. Gives `false` with the line of the swapchain. |
