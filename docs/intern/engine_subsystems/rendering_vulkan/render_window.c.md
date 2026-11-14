# src/engine_subsystems/rendering_vulkan/render_window.c

This file fits the own image to the window, and gathers the cameras of the window.

`fit_window` reads `ideatek:Swapchain` and `ideatek:VulkanSwapchain` of the window. With no image of the window in the round, the captures that wait get `IDEATEK_CAPTURE_STATUS_NO_TARGET`.
Else it fits the own image: `docs/intern/engine_subsystems/rendering_vulkan/render_targets.c.md`. A refused image writes its line once for each size: `docs/usage/engine_subsystems/rendering_vulkan.md`, *The messages*.

`gather_cameras` puts the cameras of the window that are active in the order of `order`, by insertion. A camera on a texture entity is not drawn in this version.

## Functions

| Function | Result |
| --- | --- |
| `fit_window` | `true` when the window has an image and the own image fits it. |
| `gather_cameras` | The count of the active cameras of the window, sorted by `order` into the array. |
