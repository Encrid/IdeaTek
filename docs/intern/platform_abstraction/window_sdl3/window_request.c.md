# src/platform_abstraction/window_sdl3/window_request.c

This file holds the observer `request`, which hears `ideatek:WindowRequest`, and the steps that apply a request to the window.
The roles of the request are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *The roles*.

## The observer

`bridge_request` reads the list of its triggers with `bridge_native_get_list` of BB-Native, and gives each trigger to `see_trigger`:

```pseudo
FUNCTION see_trigger(id : U64, events : U32) RETURNS I32
   IF events = BRIDGE_ON_LEAVE THEN
      IF id = window_module.entity THEN
         IF window_module.window <> nullptr THEN
            CALL SDL_HideWindow(window_module.window)
         ENDIF
         RETURN BRIDGE_END
      ENDIF
      RETURN 0
   ENDIF
   IF window_module.entity = 0 THEN
      window_module.entity <- id
   ENDIF
   IF id = window_module.entity THEN
      window_module.pending <- TRUE
   ELSE
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the entity <id> asks for a second window, and gets none")
   ENDIF
   RETURN 0
ENDFUNCTION
```

`bridge_request` gives `BRIDGE_END` if a trigger gave it, else 0.
The observer opens no window and calls no other function of SDL3, because it can run at the barrier of any phase.
Thus the system `events` applies the request in the next first phase, after it read the displays of the round.

The entity of the window leaves the terms when the game destroys it, or when the game detaches its request.
In both cases the module hides the window and ends the run. It destroys the window in `shutdown`.

## The apply

`apply_request` reads the request of the entity of the window with `get_cell`: `docs/intern/platform_abstraction/window_sdl3/window_world.c.md`.
It does these steps:

1. If the entity has no request, it changes nothing.
2. It copies the title with `cut_text`, which keeps at most 127 bytes and ends the title with a zero.
3. If the window is not open, it opens the window: `docs/intern/platform_abstraction/window_sdl3/window_open.c.md`, *The open*.
4. If the window was open before step 3 and the title changed, it calls `SDL_SetWindowTitle`.
5. If the window was open before step 3 and a different field changed, it sets the mode again with `set_mode`. It also does this when `moved` is `true`.
6. It keeps the request in `applied`, and sets `pending` and `moved` to `false` and `dirty` to `true`.

A title without a zero in its 128 bytes is longer than the title of the contract.
Then step 2 writes `the title is longer than 127 bytes, and the window shows its first <count> bytes`.
The module never reads the result of a change from the request. `attach_state` reads the window from SDL3 after the change.

## The display and the mode

`get_target` gives the display that shows the window:

- With `IDEATEK_DISPLAY_TARGET_CHOSEN`, the display of the entity `display`, if `find_display` finds it.
- Else the display of `SDL_GetPrimaryDisplay`.
- Null if no display has an entity. Then SDL3 places the window.

`get_mode` gives the mode that the window can use, and the video mode of SDL3 for `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`:

```pseudo
FUNCTION get_mode(request : ideatek_window_request_t, display : window_display_t, BYREF exclusive : SDL_DisplayMode) RETURNS ideatek_window_mode_t
   exclusive <- nullptr
   IF request.mode <> IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN THEN
      RETURN request.mode
   ENDIF
   IF display <> nullptr AND display.listed > 0 AND request.video_mode = 0 THEN
      exclusive <- SDL_GetCurrentDisplayMode(display.id)
   ENDIF
   IF display <> nullptr AND request.video_mode <> 0 THEN
      exclusive <- find_mode(display, request.video_mode)
   ENDIF
   IF exclusive = nullptr THEN
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the video mode <id> is not an exclusive mode of the display, so the window fills the display")
      RETURN IDEATEK_WINDOW_MODE_FULLSCREEN
   ENDIF
   RETURN IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN
ENDFUNCTION
```

`find_mode` gives null for a dead entity, and for a mode of a different display: `docs/intern/platform_abstraction/window_sdl3/window_video_modes.c.md`, *The finds*.
A display with no mode in the list of SDL3 has no exclusive mode, so the current mode is no exclusive mode there.
`set_mode` writes the line of a mode that the system refuses: `docs/intern/platform_abstraction/window_sdl3/window_open.c.md`, *The mode*.

## The branches

| Case | Result |
| --- | --- |
| The first entity gets `ideatek:WindowRequest`. | The next first phase opens the window. |
| A second entity gets `ideatek:WindowRequest`. | The line of the second window. No window opens. |
| The game attaches the request again. | The next first phase applies the changed fields. |
| The game destroys the entity of the window, or detaches its request. | The window hides, and the observer gives `BRIDGE_END`. |
| A different entity leaves the terms. | Nothing. |
| A title of 128 bytes with no zero. | The line of the long title, and a title of at most 127 bytes. |
| `IDEATEK_DISPLAY_TARGET_CHOSEN` with a dead display. | The primary display. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` with `video_mode` at 0. | The current mode of the display. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` with a dead mode, a mode of a different display, or a display with no mode. | The line of the video mode, and `IDEATEK_WINDOW_MODE_FULLSCREEN`. |
| The display of the window goes. | The window moves to the primary display. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_request` | Gives each trigger to `see_trigger`. Gives `BRIDGE_END` if the window entity left, else 0. |
| `see_trigger` | Keeps the entity of the window, marks its request, or writes the line of a second window. |
| `apply_request` | Opens the window, or applies the fields of the request that changed. |
| `get_target` | Gives the display that shows the window, or null. |
| `get_mode` | Gives the mode that the window can use, and its video mode of SDL3. |
