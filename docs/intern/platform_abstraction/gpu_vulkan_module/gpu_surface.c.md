# src/platform_abstraction/gpu_vulkan_module/gpu_surface.c

This file makes the surface of the window from its handles, and follows the state of the window.

## The surface

`find_maker` looks for the kind of the handles in `MAKERS`, and loads the call of that kind by its name:

| Kind | Call | First handle |
| --- | --- | --- |
| `IDEATEK_SURFACE_WINDOWS` | `vkCreateWin32SurfaceKHR` | `native_display` |
| `IDEATEK_SURFACE_METAL` | `vkCreateMetalSurfaceEXT` | `native_window` |
| `IDEATEK_SURFACE_XLIB` | `vkCreateXlibSurfaceKHR` | `native_display` |
| `IDEATEK_SURFACE_WAYLAND` | `vkCreateWaylandSurfaceKHR` | `native_display` |

A kind with no call, and a refused surface, write the same line `no surface: <result>`. The next round tries again.
The infos of the 4 calls start with the same fields, so 1 struct `gpu_surface_info_t` fills each of them.

## The window

The observer `window` keeps the entity and the handles of the window. A round can draw into a window that is not minimized and has a pixel size above 0.
New handles destroy the old surface. A new pixel size marks the swapchain as stale.
A detach of the window destroys the surface and forgets the window.

## The branches

| Case | Result |
| --- | --- |
| No call of the kind. | The line of the surface, and a surface in a later round. |
| The window goes. | Its surface goes, and the run ends. |

## Functions

| Function | Result |
| --- | --- |
| `drop_surface` | Destroys the swapchain and the surface. |
| `find_maker` | Gives the call that makes the surface of the kind, and fills the info. |
| `make_surface` | Makes the surface. Gives `false` with the line of the surface. |
| `take_state` | Keeps the handles and the size of the window. |
| `window` | Follows the state of the window. |
