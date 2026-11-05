# src/platform_abstraction/window_sdl3/window_state.c

This file finds the video driver, writes the handles of the window, and attaches `ideatek:WindowState`.
The fields of the state are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *ideatek:WindowState*.

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
Bridge calls the `shutdown` of each module in the order of the loads: https://github.com/Encrid/Bridge/blob/v0.4.0/docs/extern/bridge.h.md#bridge_destroy.
Thus the deployment loads the GPU module before the window module.

## The state

`build_state` reads the window from SDL3 into a copy of `state`, which keeps the handles:

| Field | Source |
| --- | --- |
| `display` | The entity of `find_display` for `SDL_GetDisplayForWindow`, or 0. |
| `video_mode` | With an exclusive mode, the entity of `find_mode_entity` for `SDL_GetWindowFullscreenMode`. Else 0. |
| `width` and `height` | `SDL_GetWindowSize`. |
| `pixel_width` and `pixel_height` | `SDL_GetWindowSizeInPixels`. |
| `display_scale` | `SDL_GetWindowDisplayScale`. |
| `sdr_white_level`, `hdr_headroom` and `hdr` | `read_hdr`. |
| `drop_x`, `drop_y` and `drop_over` | The fields of `window_module` that `take_drops` writes. |
| `mode` | `IDEATEK_WINDOW_MODE_WINDOWED` without `SDL_WINDOW_FULLSCREEN`. With it, `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` if `SDL_GetWindowFullscreenMode` is not null, else `IDEATEK_WINDOW_MODE_FULLSCREEN`. |
| `focused` | `SDL_WINDOW_INPUT_FOCUS` of `SDL_GetWindowFlags`. |
| `minimized` | `SDL_WINDOW_MINIMIZED` of `SDL_GetWindowFlags`. |
| `close_asked` | `close_asked` of `window_module`. |

`read_hdr` reads `SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN` of the properties of the window.
With `true`, it reads `SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT` and `SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT`, with 1.0 as the default.
With `false`, it writes 1.0 into both.

## The attach

`attach_state` attaches the state only when a byte of it changed:

```pseudo
PROCEDURE attach_state()
   DECLARE next : ideatek_window_state_t
   IF window_module.window <> nullptr AND window_module.dirty AND has_room(0, 104) THEN
      next <- window_module.state
      CALL build_state(next)
      IF next <> window_module.state THEN
         CALL BRIDGE_NATIVE_ATTACH(window_module.entity, STATE, next)
         window_module.state <- next
      ENDIF
      window_module.dirty <- FALSE
   ENDIF
ENDPROCEDURE
```

The comparison is of the 80 bytes of the 2 values. The module writes zeros into the bytes 75 to 79, so they never differ.
`state` is zero until the first attach, and `sdr_white_level` of a built state is never 0. Thus the first round of the window attaches.
An attach takes 104 bytes of `changes`. With no room, `dirty` stays `true`, and the next round tries again.

## The branches

| Case | Result |
| --- | --- |
| Each row of *The driver*. | `surface`, `native_display` and `native_window` of that row. |
| A driver that the table does not name. | `IDEATEK_SURFACE_NONE` and 2 handles of 0. |
| A driver of the table, and SDL3 gives no handle of the window. | The line of the handle, and `IDEATEK_SURFACE_NONE`. |
| A window in each mode. | `mode`, and `video_mode` only for the exclusive mode. |
| A display with HDR, and a display without it. | `hdr`, and 1.0 in the 2 levels without HDR. |
| An event of the window that changes nothing. | No attach. |
| An event that changes a field. | 1 attach. |
| No room in `changes`. | No attach in the round. |

## Functions

| Function | Result |
| --- | --- |
| `find_driver` | Gives the row of the video driver of SDL3. |
| `read_handles` | Writes the kind of surface and the handles of the window, or `IDEATEK_SURFACE_NONE` after the line. |
| `build_state` | Reads the window from SDL3 into a value of `ideatek:WindowState`. |
| `read_hdr` | Reads `hdr`, `sdr_white_level` and `hdr_headroom` of the window. |
| `attach_state` | Attaches `ideatek:WindowState` when a byte of it changed. |
