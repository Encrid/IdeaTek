# src/platform_abstraction/input_sdl3/input_focus.c

This file gives the events of the focus, and releases each input that is down when the window loses the keyboard focus: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The focus*.

## The gain

`spawn_focus` spawns an `ideatek:InputEvent` of a kind, with its rank.
`bridge_events` calls it with `IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED` before the events of the round: `docs/intern/platform_abstraction/input_sdl3/input_events.c.md`.
A focus event needs 1 spawn and 32 bytes.

## The loss

`release_all` does these steps, in this order:

1. `release_keys`: `docs/intern/platform_abstraction/input_sdl3/input_keys.c.md`, *The release*.
2. `release_buttons`: an `ideatek:MouseButton` with `down` at `false` for each bit of `next_mouse.buttons`, at the place of the mouse. Then the mask is 0.
3. `cancel_fingers`: `docs/intern/platform_abstraction/input_sdl3/input_fingers.c.md`, *The fingers that are down*.
4. `release_pads`: `docs/intern/platform_abstraction/input_sdl3/input_pad_state.c.md`, *The release*.
5. `spawn_focus` with `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST`.

`release_all` first counts the events of the 5 steps. If the round has no room for all of them, it gives `false`, and `bridge_events` keeps the focus of the last round. Thus the next round does the whole release, and the events of the release stay together.

## The branches

| Case | Result |
| --- | --- |
| The window gets the focus. | 1 event of the gain. |
| The window loses the focus with a key, a button, a finger and a gamepad button down. | 4 releases, then 1 event of the loss. |
| The window loses the focus with nothing down. | 1 event of the loss. |
| A loss with no room for all its events. | The next round does the whole release. |

## Functions

| Function | Result |
| --- | --- |
| `spawn_focus` | Spawns an event of the focus. |
| `release_buttons` | Spawns the release of the mouse buttons that are down. |
| `count_downs` | Gives the number of events of a release. |
| `release_all` | Releases each input that is down, then gives the loss. Gives `false` with no room. |
