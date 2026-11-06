# src/platform_abstraction/window_sdl3/window_mode_finds.c

This file finds the entity of a mode of SDL3, and the mode of SDL3 of an entity, among the modes of a display.

## The finds

`find_mode_entity` gives the entity of the mode of the display that has the same `w`, `h`, `pixel_density`, `refresh_rate_numerator` and `refresh_rate_denominator`.
It gives 0 for a null mode, and for a mode that the display does not have.

`find_mode` gives the mode of SDL3 of an entity, among the first `listed` modes of the display.
It gives null for an entity of a different display, for a dead entity, for a mode outside the list, and for 0.
Thus only a mode of the list of step 1 can be an exclusive mode of the window.

## The branches

| Case | Result |
| --- | --- |
| `find_mode` with an entity of a different display. | Null. |
| `find_mode` with the entity of the desktop mode outside the list. | Null. |

## Functions

| Function | Result |
| --- | --- |
| `find_mode_entity` | Gives the entity of a mode of SDL3, or 0. |
| `find_mode` | Gives the exclusive mode of SDL3 of an entity, or null. |
