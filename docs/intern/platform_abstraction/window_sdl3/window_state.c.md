# src/platform_abstraction/window_sdl3/window_state.c

This file finds the video driver, writes the handles of the window, and attaches `ideatek:WindowState`.
The fields of the state are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *ideatek:WindowState*.

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
   IF window_module.window <> nullptr AND window_module.dirty AND has_room(0, CHANGE_BYTES(ideatek_window_state_t)) THEN
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
An attach takes `CHANGE_BYTES` of `ideatek:WindowState` from `changes`. With no room, `dirty` stays `true`, and the next round tries again.

## The branches

| Case | Result |
| --- | --- |
| A window in each mode. | `mode`, and `video_mode` only for the exclusive mode. |
| A display with HDR, and a display without it. | `hdr`, and 1.0 in the 2 levels without HDR. |
| An event of the window that changes nothing. | No attach. |
| An event that changes a field. | 1 attach. |
| No room in `changes`. | No attach in the round. |

## Functions

| Function | Result |
| --- | --- |
| `read_mode` | Reads the display, the mode and the video mode of the window. |
| `build_state` | Reads the window from SDL3 into a value of `ideatek:WindowState`. |
| `read_hdr` | Reads `hdr`, `sdr_white_level` and `hdr_headroom` of the window. |
| `attach_state` | Attaches `ideatek:WindowState` when a byte of it changed. |
