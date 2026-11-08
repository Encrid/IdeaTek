# src/platform_abstraction/gpu_vulkan_module/gpu_window.c

This file follows the state of the window.

## The window

The observer `window` keeps the entity and the handles of the window. A round can draw into a window that is not minimized and has a pixel size above 0.
New handles destroy the old surface. A new pixel size marks the swapchain as stale.
`window_hdr` keeps `hdr` of the window for the choice of the format.

A detach of the window destroys the surface and forgets the window.
A second entity with `ideatek:WindowState` writes the line of the second window, and changes nothing. A detach of that entity changes nothing.

## The branches

| Case | Result |
| --- | --- |
| A second window. | The line of the second window. |
| The window goes. | Its surface goes, and the run ends. |

## Functions

| Function | Result |
| --- | --- |
| `take_state` | Keeps the handles, the size and `hdr` of the window. |
| `follow` | Reads 1 trigger of the observer: the window goes, a second window, or a state of the window. |
| `window` | Follows the state of the window. |
