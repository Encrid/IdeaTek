# src/platform_abstraction/gpu_vulkan_module/gpu_surface.c

This file makes the surface of the window from its handles.

## The window

`on_window` runs when the window module writes `ideatek:WindowState`:

1. It keeps the entity of the window and the kind of the handles `surface`.
2. A window with `IDEATEK_SURFACE_NONE` gets no surface, and no swapchain.
3. The window of the round before is forgotten. Its old surface is destroyed when the handles change, before a new one comes.

`on_swap` runs when the game writes `ideatek:SwapchainRequest` on the entity of the window. It keeps the present mode and the `hdr` flag, and asks for the swapchain.

## The making

`make_surface` calls the extension of the system with the 2 handles:

| Kind | Call |
| --- | --- |
| `IDEATEK_SURFACE_WINDOWS` | `vkCreateWin32SurfaceKHR` |
| `IDEATEK_SURFACE_METAL` | `vkCreateMetalSurfaceEXT` |
| `IDEATEK_SURFACE_XLIB` | `vkCreateXlibSurfaceKHR` |
| `IDEATEK_SURFACE_WAYLAND` | `vkCreateWaylandSurfaceKHR` |

The module turns on the extension of the system at the instance.
A kind that the table does not name, like `IDEATEK_SURFACE_XCB` or `IDEATEK_SURFACE_WEB`, writes `no surface: the kind <kind> is not there`.
A refusal writes `no surface: <reason>`.
The swapchain of the round after the surface uses it. The surface comes with the state of the window, and the swapchain with the request of the game.
The 2 observers can fire in any order.

## The branches

| Case | Result |
| --- | --- |
| The window shares no handle. | No surface, and the swapchain is skipped. |
| A kind outside the table. | The line of the surface. |
| The window closes. | The surface goes away, and the swapchain with it. |
| The state of the window changes its size. | The swapchain is remade in the next round. |

## Functions

| Function | Result |
| --- | --- |
| `on_window` | Keeps the entity of the window and makes or drops its surface. |
| `on_swap` | Keeps the request of the swapchain of the window. |
| `make_surface` | Makes the `VkSurfaceKHR` from the 2 handles of the system. |
| `drop_surface` | Destroys the surface and the swapchain of the window. |
