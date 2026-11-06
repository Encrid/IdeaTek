# src/platform_abstraction/window_sdl3/window_display_value.c

This file reads the value of each display, attaches it when a byte changed, and finds a display.

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

A name of more than 127 bytes keeps its first whole code points. Then the line of the long name comes.
`update_display` attaches the value only when a byte of it differs from `attached`, with 184 bytes of room.

## The find

`find_display` gives the display whose `id` is the `SDL_DisplayID` that it gets, or whose `entity` is the entity that it gets.
The caller gives 0 for the other one. No display has an id or an entity of 0, so 0 never matches.
It gives null when no display matches, for example for a display that went.

## The branches

| Case | Result |
| --- | --- |
| The primary display changes. | The 2 displays attach with their new `is_primary`. |
| A change of a mode, a scale or a size of a display. | The display attaches again. |
| The list of modes of a display changes. | New entities for its modes. |
| A name of more than 127 bytes. | The line of the long name, and a name cut at a whole code point. |

## Functions

| Function | Result |
| --- | --- |
| `update_display` | Reads a display again, and attaches its value when a byte changed. |
| `find_display` | Gives the display of an `SDL_DisplayID` or of an entity, or null. |
| `has_new_modes` | Tells if the list of the modes of a display changed. |
| `renew_modes` | Spawns new entities for the modes of a display whose list changed. |
| `read_display` | Reads a display from SDL3 into a value of `ideatek:Display`. |
