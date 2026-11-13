# src/engine_subsystems/rendering_vulkan/render_targets.c

This file keeps the own image of the window, the image that the cameras draw into, and turns a rect into pixels.

The image of the swapchain has no `VK_IMAGE_USAGE_TRANSFER_SRC_BIT`, so no capture can copy it.
So the cameras draw into an image of the module. At the end of the round, the module copies it to the window: `docs/intern/engine_subsystems/rendering_vulkan/render_copy.c.md`.

The image has the format `VK_FORMAT_R8G8B8A8_SRGB`, and the usages of a color attachment, a source and a target of a copy. Its memory is local to the device.
Its size is `width` and `height` of `ideatek:Swapchain`. When they change, `fit_target` makes the image again.
A refused image keeps the size of its try, so the module tries again only at a new size.

`get_area` turns a rect of `ideatek_rect_t`, in fractions, into pixels of the image. It keeps the rect inside the image.
A zero width or height covers the rest of the image from the corner, as `viewport` of `ideatek:Camera` tells.

## Functions

| Function | Result |
| --- | --- |
| `get_area` | The pixels of a rect of fractions, inside the image. |
| `make_image` | Makes the image, its memory and its view. `false` when Vulkan refuses. |
| `fit_target` | Keeps the image at the size of the swapchain. `false` when the image of that size is refused. |
| `free_target` | Frees the image, its view and its memory. |
