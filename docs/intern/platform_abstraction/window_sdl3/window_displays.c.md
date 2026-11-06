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

## The value of a display

`update_display` reads the list of modes again.
If the count of the list changed, or `find_mode_entity` finds no entity for a mode of the list, the list changed.
Then it destroys the old modes, and spawns the new modes, with the room of both.

Then it fills a value of `ideatek:Display`:

| Field | Source |
| --- | --- |
| `name` | `SDL_GetDisplayName`, copied with `cut_text`. |
| `desktop_mode` | `find_mode_entity` for `SDL_GetDesktopDisplayMode`. |
| `current_mode` | `find_mode_entity` for `SDL_GetCurrentDisplayMode`. |
| `width` and `height` | `SDL_GetDisplayBounds`. |
| `content_scale` | `SDL_GetDisplayContentScale`. |
| `hdr` | `SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN` of `SDL_GetDisplayProperties`. |
| `is_primary` | `true` only for the first id of the list of step 1. |

The primary display of SDL3 is the first display of its list: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/video/SDL_video.c#L1006.
Thus exactly 1 display entity has `is_primary` at `true`, if 1 display has an entity.

A name of more than 127 bytes keeps its first whole code points, and the module writes the line of the long name.
`update_display` attaches the value only when a byte of it differs from `attached`, with 184 bytes of room.

## The find

`find_display` gives the display whose `id` is the `SDL_DisplayID` that it gets, or whose `entity` is the entity that it gets.
The caller gives 0 for the other one. No display has an id or an entity of 0, so 0 never matches.
It gives null when no display matches, for example for a display that went.

## The branches

| Case | Result |
| --- | --- |
| The first round. | An entity for each display, and for each of its modes. |
| The system adds a display. | Its entity and the entities of its modes. |
| The system removes a display. | Its entity and the entities of its modes go. |
| The system removes the display of `IDEATEK_DISPLAY_TARGET_CHOSEN`. | The window moves to the primary display. |
| A display comes back. | A new entity. The window does not move back. |
| The primary display changes. | The 2 displays attach with their new `is_primary`. |
| A change of a mode, a scale or a size of a display. | The display attaches again. |
| The list of modes of a display changes. | New entities for its modes. |
| A 17th display. | The line of the 17th display, and no entity. |
| A name of more than 127 bytes. | The line of the long name, and a name cut at a whole code point. |
| SDL3 gives no list of the displays. | The line of the list. The entities stay. |
| No room in the round. | The next round does the rest. |

## Functions

| Function | Result |
| --- | --- |
| `sync_displays` | Makes the entities of the displays match the displays of SDL3. |
| `add_display` | Spawns the entities of a new display and of its modes. |
| `remove_display` | Destroys the entities of a display that went, and of its modes. |
| `update_display` | Reads a display again, and attaches its value when a byte changed. |
| `find_display` | Gives the display of an `SDL_DisplayID` or of an entity, or null. |
