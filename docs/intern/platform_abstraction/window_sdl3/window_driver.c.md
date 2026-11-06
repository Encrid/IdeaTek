# src/platform_abstraction/window_sdl3/window_driver.c

This file finds the video driver of SDL3, and writes the handles of the window into the state of the module.

## The driver

`find_driver` compares the name that `SDL_GetCurrentVideoDriver` gives with the rows of this table, and gives the first row that matches.
`bridge_init` keeps the row in `driver`, after the start of the video.

| `name` | `surface` | `flags` | `paste` | Key of `native_display` | Key of `native_window` |
| --- | --- | --- | --- | --- | --- |
| `"windows"` | `IDEATEK_SURFACE_WINDOWS` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER` | `SDL_PROP_WINDOW_WIN32_HWND_POINTER` |
| `"cocoa"` | `IDEATEK_SURFACE_METAL` | `SDL_WINDOW_METAL` | `SDL_KMOD_GUI` | None. | None: *The handles* |
| `"x11"` | `IDEATEK_SURFACE_XLIB` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_X11_DISPLAY_POINTER` | `SDL_PROP_WINDOW_X11_WINDOW_NUMBER` |
| `"wayland"` | `IDEATEK_SURFACE_WAYLAND` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER` | `SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER` |
| Each other name | `IDEATEK_SURFACE_NONE` | 0 | `SDL_KMOD_CTRL` | None. | None. |

The last row matches each name. It serves a driver with no window of the system, for example `"offscreen"` or `"dummy"`.
SDL3 gives no connection of XCB, so the module states `IDEATEK_SURFACE_XLIB` on X11, and never `IDEATEK_SURFACE_XCB`.
The targets of the module have no Android and no web, so it never states `IDEATEK_SURFACE_ANDROID` or `IDEATEK_SURFACE_WEB`.

## The handles

`read_handles` writes `surface`, `native_display` and `native_window` into `state`, 1 time when the window opens:

```pseudo
PROCEDURE read_handles()
   DECLARE props : U32
   props <- SDL_GetWindowProperties(window_module.window)
   window_module.state.surface <- window_module.driver.surface
   window_module.state.native_display <- SDL_GetPointerProperty(props, window_module.driver.display_key, 0)
   IF window_module.driver.surface = IDEATEK_SURFACE_XLIB THEN
      window_module.state.native_window <- SDL_GetNumberProperty(props, window_module.driver.window_key, 0)
   ELSE
      window_module.state.native_window <- SDL_GetPointerProperty(props, window_module.driver.window_key, 0)
   ENDIF
   IF window_module.driver.surface = IDEATEK_SURFACE_METAL THEN
      window_module.view <- SDL_Metal_CreateView(window_module.window)
      window_module.state.native_window <- SDL_Metal_GetLayer(window_module.view)
   ENDIF
   IF window_module.driver.surface <> IDEATEK_SURFACE_NONE AND window_module.state.native_window = 0 THEN
      CALL bridge_native_say(BRIDGE_LOG_ERR, "the window has no handle of its surface: <reason>")
      window_module.state.surface <- IDEATEK_SURFACE_NONE
      window_module.state.native_display <- 0
   ENDIF
ENDPROCEDURE
```

A key of None gives 0. On X11 the window is a number, and on each other system a handle is a pointer.
`SDL_Metal_GetLayer` gives null for a null view. Then the last test writes the line, and states `IDEATEK_SURFACE_NONE`.
Thus a reader never gets a kind of surface with a handle of 0.

The GPU module makes its surface from the handles, and the surface must go before the window.
Bridge calls the `shutdown` of each module in the order of the loads: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_destroy.
Thus the GPU module loads first: `docs/extern/platform_abstraction/window_sdl3/readme.md`, *The deployment*.

## The branches

| Case | Result |
| --- | --- |
| Each row of *The driver*. | `surface`, `native_display` and `native_window` of that row. |
| A driver that the table does not name. | `IDEATEK_SURFACE_NONE` and 2 handles of 0. |
| A driver of the table, and SDL3 gives no handle of the window. | The line of the handle, and `IDEATEK_SURFACE_NONE`. |

## Functions

| Function | Result |
| --- | --- |
| `find_driver` | Gives the row of the video driver of SDL3. |
| `read_handles` | Writes the kind of surface and the handles of the window, or `IDEATEK_SURFACE_NONE` after the line. |
| `check_handles` | Writes the line of a missing handle, and states `IDEATEK_SURFACE_NONE`. |
