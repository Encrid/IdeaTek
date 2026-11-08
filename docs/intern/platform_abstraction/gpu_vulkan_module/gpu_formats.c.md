# src/platform_abstraction/gpu_vulkan_module/gpu_formats.c

This file picks the format, the present mode and the count of the images of a swapchain from the lists of a surface.

## The format

With no HDR, the format is `VK_FORMAT_B8G8R8A8_SRGB` with the color space of sRGB.
With HDR, the first pair of `HDR` that the surface lists wins: 2 formats of HDR10, then the extended sRGB linear. A format in a different color space does not count.

## The present mode

The mode of the request stays when the surface lists it. Else, and for a value outside the enum, the mode is vsync.

## The count

`clamp_count` puts a count into a range, with no branch.

## The branches

| Case | Result |
| --- | --- |
| HDR on a surface with no format of HDR. | sRGB. |
| HDR on a surface with HDR10. | HDR10. |
| A format of HDR10 in the color space of sRGB. | Not taken. |
| A mode that the surface lists. | The mode. |
| A mode with no place in the list, or a value outside the enum. | Vsync. |

## Functions

| Function | Result |
| --- | --- |
| `pick_format` | Gives the format and the color space for a request. |
| `pick_present` | Gives the present mode for a request. |
| `vulkan_present` | Gives the mode of Vulkan of a mode of the contract. |
| `clamp_count` | Gives a count in a range. |
