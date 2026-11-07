# src/platform_abstraction/gpu_vulkan_module/gpu_swapchain.c

This file makes and remakes the swapchain of the window.

## The making

`make_swapchain` runs when a device is ready, a surface is there and a request came, or when the pixel size of the window changed:

1. It reads the capacities of the surface for the size of the window. `image_count` is clamped into the range of the surface.
2. It picks the format. With `hdr` it takes the first format of HDR10, then of extended sRGB linear, that the surface lists. Else, or with no format of HDR, it takes `VK_FORMAT_B8G8R8A8_SRGB` with the color space of sRGB, and writes `no hdr on the window: srgb`.
3. It picks the present mode of the request among the modes of the surface. A mode that the surface lacks gives the line of the present mode, and the fifo takes its place.
4. It makes the swapchain and 1 view of the whole image for each image.
5. It attaches `ideatek:Swapchain` and `ideatek:VulkanSwapchain` on the entity of the window. `frame` and `slot` stay 0 until the first round takes an image.

A refusal writes `no swapchain: <reason>`. The window gets the status `IDEATEK_SWAPCHAIN_STATUS_SKIPPED` in each round until its size changes.

```pseudo
FUNCTION choose_format() RETURNS PAIR
   IF hdr THEN
      FOR i <- 0 TO format_count - 1
         IF formats[i].space = HDR10 THEN
            RETURN formats[i]
         ENDIF
      NEXT i
   ENDIF
   RETURN bgra8_srgb
ENDFUNCTION
```

## The remake

`drop_surface` and the resize path destroy the swapchain, its views and the old values, after a wait for the device.
The module keeps the pixel size of the last swapchain; a state that repeats it changes nothing.

## The branches

| Case | Result |
| --- | --- |
| `image_count` under the least of the surface. | The least of the surface is used. |
| `image_count` over the most of the surface. | The most of the surface is used. |
| `hdr` and the surface lists HDR10. | The format and the space of HDR10. |
| `hdr` and the surface lists no format of HDR. | The line of the hdr, and the sRGB pair. |
| The asked present mode is not there. | The line of the present mode, and the fifo. |
| The size of the window repeats. | No new swapchain. |
| The creation is refused. | The line of the swapchain, and the status `SKIPPED`. |

## Functions

| Function | Result |
| --- | --- |
| `make_swapchain` | Makes the swapchain and its views, and attaches the values. |
| `choose_format` | Gives the format and the color space for the request. |
| `choose_present` | Gives the present mode for the request, or the fifo. |
| `drop_swapchain` | Destroys the swapchain and its views. |
