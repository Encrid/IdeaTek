# src/platform_abstraction/window_sdl3/window_open.c

This file opens the window of SDL3, sets its icon and its mode, and closes it.
The module has at most 1 window, so the window is `window` of `window_module`.

## The open

`open_window` opens the window of a request:

1. It calls `SDL_CreateWindow` with the title, `width`, `height` and the flags of the open.
2. If SDL3 refuses, it writes the line of no window. The window stays closed until the game attaches a request again.
3. `set_icon` sets the icon of the setting `icon`.
4. `read_handles` writes the handles of the window: `docs/intern/platform_abstraction/window_sdl3/window_handles.c.md`, *The handles*.
5. `set_mode` sets the display and the mode of the window.
6. `SDL_ShowWindow` shows the window.

The flags of the open are `SDL_WINDOW_HIDDEN`, `SDL_WINDOW_RESIZABLE`, `SDL_WINDOW_HIGH_PIXEL_DENSITY`, and `flags` of the row of the driver.
The window stays hidden until step 6. Thus the user does not see the window move from one display or mode to the next.
`SDL_WINDOW_HIGH_PIXEL_DENSITY` asks for all the pixels of the display, so `pixel_width` can be more than `width`.

The row of the driver of macOS has `SDL_WINDOW_METAL` in `flags`, and each other row has 0.
Thus the window names a graphics API on macOS, and SDL3 loads no GL: `docs/usage/platform_abstraction/sdl3.md`, *Usage*.
On the other systems, a window with no graphics flag loads no library of a graphics API.
The module makes no surface and no context: the GPU module makes the surface from the handles.

## The icon

`set_icon` reads the setting `icon` with `copy_arena_text`: `docs/intern/platform_abstraction/window_sdl3/window_text.c.md`.

```pseudo
PROCEDURE set_icon()
   DECLARE settings : window_settings_t
   DECLARE path : STRING
   DECLARE surface : SDL_Surface
   settings <- bridge_native_get_single(SETTINGS)
   path <- copy_arena_text(settings.icon)
   IF path <> nullptr AND path <> "none" THEN
      surface <- SDL_LoadPNG(path)
      IF surface = nullptr OR NOT SDL_SetWindowIcon(window_module.window, surface) THEN
         CALL bridge_native_say(BRIDGE_LOG_WARN, "no icon <path>: <reason>")
      ENDIF
      CALL SDL_DestroySurface(surface)
   ENDIF
   CALL free(path)
ENDPROCEDURE
```

`SDL_DestroySurface` and `free` do nothing with null.
The window keeps the icon of the system after the line. The icon never changes after the open.
A system with no icon of a window refuses `SDL_SetWindowIcon`, for example Wayland without the protocol of the icons.

## The mode

`set_mode` puts the window on the display of `get_target`: `docs/intern/platform_abstraction/window_sdl3/window_request.c.md`, *The display and the mode*. `set_fullscreen` sets a mode that is not windowed, with the mode of `get_exclusive`.

```pseudo
PROCEDURE set_mode(request : ideatek_window_request_t, display : window_display_t)
   DECLARE exclusive : SDL_DisplayMode
   DECLARE place : I32
   place <- SDL_WINDOWPOS_CENTERED
   IF display <> nullptr THEN
      place <- SDL_WINDOWPOS_CENTERED_DISPLAY(display.id)
   ENDIF
   IF get_mode(request, display, exclusive) = IDEATEK_WINDOW_MODE_WINDOWED THEN
      CALL SDL_SetWindowFullscreen(window_module.window, FALSE)
      CALL SDL_SetWindowSize(window_module.window, request.width, request.height)
      CALL SDL_SetWindowPosition(window_module.window, place, place)
   ELSE
      CALL SDL_SetWindowPosition(window_module.window, place, place)
      IF NOT SDL_SetWindowFullscreenMode(window_module.window, exclusive) THEN
         CALL bridge_native_say(BRIDGE_LOG_WARN, "the system refuses the video mode <id>: <reason>, so the window fills the display")
         CALL SDL_SetWindowFullscreenMode(window_module.window, nullptr)
      ENDIF
      CALL SDL_SetWindowFullscreen(window_module.window, TRUE)
   ENDIF
ENDPROCEDURE
```

A mode of null is the mode `IDEATEK_WINDOW_MODE_FULLSCREEN`: the window fills the display, and the display keeps its mode.
SDL3 refuses no null mode, so the second call of `SDL_SetWindowFullscreenMode` always succeeds.
SDL3 can apply a mode later than the call. Then an event of the window comes, and `attach_state` reads the new mode.

## The exclusive mode

`get_exclusive` gives the video mode of SDL3 for `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`, or null:

```pseudo
FUNCTION get_exclusive(request : ideatek_window_request_t, display : window_display_t) RETURNS SDL_DisplayMode
   DECLARE exclusive : SDL_DisplayMode
   exclusive <- nullptr
   IF display <> nullptr AND display.listed > 0 AND request.video_mode = 0 THEN
      exclusive <- SDL_GetCurrentDisplayMode(display.id)
   ENDIF
   IF display <> nullptr AND request.video_mode <> 0 THEN
      exclusive <- find_mode(display, request.video_mode)
   ENDIF
   IF exclusive = nullptr THEN
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the video mode <id> is not an exclusive mode of the display, so the window fills the display")
   ENDIF
   RETURN exclusive
ENDFUNCTION
```

A mode of null makes the window fill the display. `find_mode` gives null for a dead entity, and for a mode of a different display: `docs/intern/platform_abstraction/window_sdl3/window_mode_finds.c.md`, *The finds*.
A display with no mode in the list of SDL3 has no exclusive mode, so the current mode is no exclusive mode there.

## The branches

| Case | Result |
| --- | --- |
| SDL3 opens the window. | The window shows, with its title, its size, its display and its mode. |
| SDL3 refuses the window. | The line of no window, and no `ideatek:WindowState`. |
| `icon` is `"none"`. | The icon of the system. |
| `icon` names a PNG file. | The icon of the file. |
| `icon` names no PNG file, or the system refuses the icon. | The line of the icon, and the icon of the system. |
| The mode `IDEATEK_WINDOW_MODE_WINDOWED`. | The window has its size, in the middle of its display. |
| The mode `IDEATEK_WINDOW_MODE_FULLSCREEN`. | The window fills its display. |
| An exclusive mode that the system takes. | The display uses the mode while the window has the focus. |
| An exclusive mode that the system refuses. | The line of the refused mode, and the window fills the display. |
| No display has an entity. | SDL3 places the window at the center of its display. |
| No memory for the path of the icon. | The line of the memory, and the icon of the system. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` and no display has an entity. | The line of the video mode, and the window fills the display of SDL3. |

## Functions

| Function | Result |
| --- | --- |
| `open_window` | Opens the window of a request, hidden, then shows it. Gives `true`, or `false` after the line of no window. |
| `set_icon` | Sets the icon of the setting `icon`, or writes the line of the icon. |
| `set_mode` | Puts the window on its display, in its mode. |
| `get_exclusive` | Gives the exclusive mode of the request, or null after the line of the video mode. |
| `set_fullscreen` | Fills the display, in the exclusive mode if the system takes it. |
