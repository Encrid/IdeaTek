# src/kernel/frame_sdl3/frame_window.c

This file follows the entity of the window.

## The window

The observer `window` sees each write and each detach of `ideatek:WindowState`.
The first entity becomes the window. A detach of that entity forgets it.
A second entity changes nothing: the window module serves 1 window.
The write of `ideatek:EventWait` is in `docs/intern/kernel/frame_sdl3/frame_round.c.md`, where the settings are known.

## The branches

| Case | Result |
| --- | --- |
| The first window. | The entity is kept. |
| The window goes. | The entity is 0. |
| A second window. | No change. |

## Functions

| Function | Result |
| --- | --- |
| `follow` | Reads 1 trigger of the observer. |
| `window` | Follows the state of the window. |
