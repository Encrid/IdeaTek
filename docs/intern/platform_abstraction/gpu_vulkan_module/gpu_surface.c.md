# src/platform_abstraction/gpu_vulkan_module/gpu_surface.c

This file makes the surface of the window from its handles.

## The maker

`find_maker` looks for the kind of the handles in `MAKERS`, and loads the call of that kind by its name:

| Kind | Call | First handle |
| --- | --- | --- |
| `IDEATEK_SURFACE_WINDOWS` | `vkCreateWin32SurfaceKHR` | `native_display` |
| `IDEATEK_SURFACE_METAL` | `vkCreateMetalSurfaceEXT` | `native_window` |
| `IDEATEK_SURFACE_XLIB` | `vkCreateXlibSurfaceKHR` | `native_display` |
| `IDEATEK_SURFACE_WAYLAND` | `vkCreateWaylandSurfaceKHR` | `native_display` |

A kind with no call, and a refused surface, write the same line `no surface: <result>`. The next round tries again.
The infos of the 4 calls start with the same fields, so 1 struct `gpu_surface_info_t` fills each of them.

## The present

`can_present` asks whether the graphics queue can show images on the surface.
A result below 0, or no, writes `no present on the queue: <result>`, destroys the surface and sets `no_present`.
The module makes no surface for the window while `no_present` is set. New handles of the window clear it.

## The branches

| Case | Result |
| --- | --- |
| No call of the kind. | The line of the surface, and a surface in a later round. |
| The queue cannot show on the surface. | The line of the present, no surface, and the status skipped. |

## Functions

| Function | Result |
| --- | --- |
| `find_maker` | Gives the call that makes the surface of the kind, and fills the info. |
| `can_present` | Checks that the graphics queue can show on the surface. Gives `false` with the line of the present. |
| `make_surface` | Makes the surface. Gives `false` with the line of the surface or of the present. |
