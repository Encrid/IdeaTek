# src/engine_subsystems/rendering_vulkan/render_copy.c

This file records the changes of layout of an image, and the copy of the own image to the window.

`move_image` records 1 barrier that moves an image from 1 layout to another. Its stages are all the commands, as the work of a round is small.
`copy_target` moves the image of the swapchain to the layout of a target of a copy. It records a blit from the own image, and moves it back.
The blit also turns RGBA into the order of the format of the swapchain, as BGRA. The image of the swapchain ends in the layout that the GPU module needs at its show.

## Functions

| Function | Result |
| --- | --- |
| `move_image` | Records the barrier of 1 change of layout. |
| `copy_target` | Records the blit of the own image to the image of the swapchain, with its 2 barriers. |
