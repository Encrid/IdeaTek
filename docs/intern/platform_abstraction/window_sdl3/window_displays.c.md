# src/platform_abstraction/window_sdl3/window_displays.c

This file makes the entities of `ideatek:Display` match the displays of SDL3: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The displays*.
The module keeps each display in a `window_display_t` of `displays`: `docs/intern/platform_abstraction/window_sdl3/window_inside.h.md`, *The types*.

## The sync

`sync_displays` runs in a round when `displays_dirty` is `true`.
`displays_dirty` is `true` in the first round, and after each event of a display.

1. `SDL_GetDisplays` gives the ids of the displays. The first id is the primary display.
2. If SDL3 gives no list, it writes the line of the list, and stops.
3. For each display of `displays` whose id is not in the list, it calls `remove_display`.
4. For each id of the list with no display in `displays`, it calls `add_display`.
5. For each display of `displays`, it calls `update_display` with the id of the primary display.
6. It frees the list with `SDL_free`.

`displays_dirty` stays `true` after a step that had no room in the round: `docs/intern/platform_abstraction/window_sdl3/window_world.c.md`, *The room of a round*.
Thus the next round does the rest. Each other end of `sync_displays` sets `displays_dirty` to `false`.

## A new display

`add_display` reads the modes with `read_modes`: `docs/intern/platform_abstraction/window_sdl3/window_video_modes.c.md`.
It needs room for 1 spawn and 184 bytes for the display, and for 1 spawn and 56 bytes for each mode.
With the room, it spawns the entity of the display, calls `spawn_modes`, and adds the display at the end of `displays`.

With 16 displays in `displays`, a new display gets no entity, and the module writes the line of the 17th display.

## A display that goes

`remove_display` needs 24 bytes of room for the destroy of the display and for each of its modes.
With the room, it calls `destroy_modes`, destroys the entity of the display, and puts the last display of `displays` in its place.

If the request of the window names the display with `IDEATEK_DISPLAY_TARGET_CHOSEN`, it also sets `moved` to `true`.
Then the next apply puts the window on the primary display: `docs/intern/platform_abstraction/window_sdl3/window_request.c.md`, *The display and the mode*.
A display that comes back gets a new entity. Thus the window stays on the primary display.

## The branches

| Case | Result |
| --- | --- |
| The first round. | An entity for each display, and for each of its modes. |
| The system adds a display. | Its entity and the entities of its modes. |
| The system removes a display. | Its entity and the entities of its modes go. |
| The system removes the display of `IDEATEK_DISPLAY_TARGET_CHOSEN`. | The window moves to the primary display. |
| A display comes back. | A new entity. The window does not move back. |
| A 17th display. | The line of the 17th display, and no entity. |
| SDL3 gives no list of the displays. | The line of the list. The entities stay. |
| No room in the round. | The next round does the rest. |

## Functions

| Function | Result |
| --- | --- |
| `is_listed` | Tells if an id is in the list of the displays of SDL3. |
| `match_list` | Removes the displays that went, and adds the new ones. |
| `sync_displays` | Makes the entities of the displays match the displays of SDL3. |
| `add_display` | Spawns the entities of a new display and of its modes. |
| `remove_display` | Destroys the entities of a display that went, and of its modes. |
