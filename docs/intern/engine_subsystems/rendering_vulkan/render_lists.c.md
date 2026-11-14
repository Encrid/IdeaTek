# src/engine_subsystems/rendering_vulkan/render_lists.c

This file runs the observers `device`, `window` and `cameras`.

`device` and `window` keep 1 entity each: the last that entered or changed, or none after its leave. `get_last` reads that from the list of the call.
`cameras` keeps the set of the cameras. `change_camera` adds an entity that enters, and takes out an entity that leaves.
A set that holds `RENDER_CAMERAS` keeps no more. Its line is in `docs/usage/engine_subsystems/rendering_vulkan.md`, *The messages*.

## Functions

| Function | Result |
| --- | --- |
| `get_last` | The entity that the list of 1 observer leaves in its place. |
| `change_camera` | Adds or takes out 1 camera. Writes the line of the room when the set is full. |
| `device` | The observer of `ideatek:VulkanDevice`. Gives 0. |
| `window` | The observer of `ideatek:VulkanSwapchain`. Gives 0. |
| `cameras` | The observer of `ideatek:Camera`. Gives 0. |
