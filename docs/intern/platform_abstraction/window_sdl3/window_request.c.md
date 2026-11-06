# src/platform_abstraction/window_sdl3/window_request.c

This file holds the steps that apply a request of the system `events` to the window.
The roles of the request are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *The roles*.

## The apply

`apply_request` reads the request of the entity of the window with `get_cell`: `docs/intern/platform_abstraction/window_sdl3/window_world.c.md`.
It does these steps:

1. It sets `pending` to `false`. The entity of the window carries its request, because a detach of the request ends the run: `docs/intern/platform_abstraction/window_sdl3/window_observe.c.md`.
2. It copies the title with `cut_text`, which keeps at most 127 bytes and ends the title with a zero.
3. If the window is not open, it opens the window: `docs/intern/platform_abstraction/window_sdl3/window_open.c.md`, *The open*. If SDL3 refuses the window, it changes nothing more.
4. If the window was open before step 3 and the title changed, it calls `SDL_SetWindowTitle`.
5. If the window was open before step 3 and a different field changed, it sets the mode again with `set_mode`. It also does this when `moved` is `true`.
6. It keeps the request in `applied`, and sets `moved` to `false` and `dirty` to `true`.

A title without a zero in its 128 bytes is longer than the title of the contract.
Then step 2 writes the line of the long title.
The module never reads the result of a change from the request. `attach_state` reads the window from SDL3 after the change.

## The display and the mode

`get_target` gives the display that shows the window:

- With `IDEATEK_DISPLAY_TARGET_CHOSEN`, the display of the entity `display`, if `find_display` finds it.
- Else the display of `SDL_GetPrimaryDisplay`.
- Null if no display has an entity. Then SDL3 places the window.

`docs/intern/platform_abstraction/window_sdl3/window_open.c.md`, *The exclusive mode*, tells the video mode of the window.

## The branches

| Case | Result |
| --- | --- |
| The game attaches the request again. | The next first phase applies the changed fields. |
| A title of 128 bytes with no zero. | The line of the long title, and a title of at most 127 bytes. |
| `IDEATEK_DISPLAY_TARGET_CHOSEN` with a dead display. | The primary display. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` with `video_mode` at 0. | The current mode of the display. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` with a dead mode, a mode of a different display, or a display with no mode. | The line of the video mode, and `IDEATEK_WINDOW_MODE_FULLSCREEN`. |
| The display of the window goes. | The window moves to the primary display. |
| The game attaches the request again with a new value in 1 field. | The next first phase changes that field of the window. |
| The game attaches the request again with only a new title. | The title changes, and the mode stays. |

## Functions

| Function | Result |
| --- | --- |
| `apply_request` | Opens the window, or applies the fields of the request that changed. |
| `get_target` | Gives the display that shows the window, or null. |
| `is_new_mode` | Tells if a field of the request other than the title changed. |
| `read_request` | Copies the request, and cuts its title to 127 bytes. |
