# src/engine_subsystems/rendering_vulkan/render_observe.c

This file registers the 4 observers and the system of the module.

| Observer | Term | Events |
| --- | --- | --- |
| `device` | `ideatek:VulkanDevice` | Enter, attach and leave |
| `window` | `ideatek:VulkanSwapchain` | Enter, attach and leave |
| `cameras` | `ideatek:Camera` | Enter, attach and leave |
| `request` | `ideatek:CaptureRequest` | Enter, attach and leave |

A system has 1 query, and the module needs 4 kinds of entities. So the observers keep the entities, and `draw` reads their values by their ids.
The cost then follows the changes, not the count of the entities.

The system `draw` runs in the phase `last`, with no term. It changes `ideatek:Capture` and `ideatek:RenderStats`.

## Functions

| Function | Result |
| --- | --- |
| `add_observer` | Adds the observer of 1 row and gives its list. |
| `add_runs` | Registers the 4 observers and the system. |
