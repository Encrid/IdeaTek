# src/platform_abstraction/window_sdl3/window_handles.c

This file writes the handles of the window into the state of the module.

## The handles

`read_handles` writes `surface`, `native_display` and `native_window` into `state`, 1 time when the window opens:

```pseudo
PROCEDURE read_handles()
   DECLARE props : U32
   props <- SDL_GetWindowProperties(window_module.window)
   window_module.state.surface <- window_module.driver.surface
   window_module.state.native_display <- SDL_GetPointerProperty(props, window_module.driver.display_key, 0)
   window_module.state.native_window <- read_window(props, window_module.driver)
   IF window_module.driver.surface <> IDEATEK_SURFACE_NONE AND window_module.state.native_window = 0 THEN
      CALL bridge_native_say(BRIDGE_LOG_ERR, "the window has no handle of its surface: <reason>")
      window_module.state.surface <- IDEATEK_SURFACE_NONE
      window_module.state.native_display <- 0
   ENDIF
ENDPROCEDURE
```

A key of None gives 0. `read_window` is compiled for each system, as the rows of `docs/intern/platform_abstraction/window_sdl3/window_driver.c.md`, *The driver* are:

| System | `native_window` |
| --- | --- |
| Windows | The pointer of `window_key`. |
| macOS | For `IDEATEK_SURFACE_METAL`, the layer of a new Metal view, from `SDL_Metal_CreateView` and `SDL_Metal_GetLayer`. Else 0. |
| Linux | For `IDEATEK_SURFACE_XLIB`, the number of `window_key`, because the window of X11 is a number. Else the pointer of `window_key`. |

`SDL_Metal_GetLayer` gives null for a null view. Then the last test writes the line, and states `IDEATEK_SURFACE_NONE`.
Thus a reader never gets a kind of surface with a handle of 0.

The GPU module makes its surface from the handles, and the surface must go before the window.
Bridge calls the `shutdown` of each module in the order of the loads: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_destroy.
Thus the GPU module loads first: `docs/extern/platform_abstraction/window_sdl3/readme.md`, *The deployment*.

## The branches

| Case | Result |
| --- | --- |
| A driver of the table, and SDL3 gives no handle of the window. | The line of the handle, and `IDEATEK_SURFACE_NONE`. |

## Functions

| Function | Result |
| --- | --- |
| `read_window` | Gives the handle of the window of the driver, as the system of the build reads it. |
| `read_handles` | Writes the kind of surface and the handles of the window, or `IDEATEK_SURFACE_NONE` after the line. |
| `check_handles` | Writes the line of a missing handle, and states `IDEATEK_SURFACE_NONE`. |
