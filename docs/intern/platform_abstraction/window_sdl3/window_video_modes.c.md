# src/platform_abstraction/window_sdl3/window_video_modes.c

This file keeps the video modes of a display, and their entities of `ideatek:VideoMode`.
The modes of a display are `modes` of its `window_display_t`, `mode_count` of them. The first `listed` modes are the exclusive modes of SDL3.

## The read

`read_modes` reads the modes of a display into a list:

1. `SDL_GetFullscreenDisplayModes` gives the exclusive modes of the display, the largest first.
2. If SDL3 gives no list, the module writes the line of no video modes. The list then has no exclusive mode.
3. It copies at most 253 modes, and `listed` is their count.
4. With more modes, it writes the line of the modes.
5. It adds the desktop mode and the current mode at the end, if the list lacks them.
6. It frees the list of SDL3 with `SDL_free`.

Step 5 gives an entity to the desktop mode and to the current mode of the display, also when SDL3 does not list them.
Thus `desktop_mode` and `current_mode` of `ideatek:Display` name an entity.

## The entities

`spawn_modes` spawns an entity for each mode of a display, and attaches its `ideatek:VideoMode`:

| Field | Source |
| --- | --- |
| `display` | The entity of the display. |
| `width` and `height` | `w` and `h` of the mode. |
| `pixel_density` | `pixel_density` of the mode. |
| `refresh_numerator` | `refresh_rate_numerator` of the mode. |
| `refresh_denominator` | `refresh_rate_denominator` of the mode, or 1 when the numerator is 0. |
| `order` | The index of the mode in the list. |

The denominator of 1 keeps a refresh rate of 0 Hz with no division by 0.
`spawn_modes` does not check the room. `add_display` and `update_display` check it: `docs/intern/platform_abstraction/window_sdl3/window_displays.c.md`, *A new display*.
`destroy_modes` destroys the entity of each mode of a display, and sets `mode_count` and `listed` to 0.

## The branches

| Case | Result |
| --- | --- |
| A display with a list of modes. | An entity for each mode, with `order` from 0. |
| A display with more than 253 modes. | The line of the modes, and 253 entities for the list. |
| SDL3 gives no list of the modes. | The line of the modes. Until the next read of its modes, the display has only its desktop mode and its current mode. |
| A desktop mode that the list does not hold. | An entity for the desktop mode, after the list. |
| A mode with no refresh rate. | `refresh_numerator` of 0 and `refresh_denominator` of 1. |

## Functions

| Function | Result |
| --- | --- |
| `read_modes` | Reads the modes of a display into a list. Gives the count of the modes. |
| `add_extras` | Adds the desktop mode and the current mode after the list, if the list lacks them. |
| `spawn_modes` | Spawns and attaches an `ideatek:VideoMode` for each mode of a display. |
| `destroy_modes` | Destroys the entities of the modes of a display. |
