# src/platform_abstraction/input_sdl3/input_pad_state.c

This file keeps the sticks, the triggers and the buttons of each gamepad in its `ideatek:GamepadState`, and spawns an `ideatek:GamepadEvent` for each button: `docs/extern/platform_abstraction/ideatek_input_gamepad.h.md`.

## The axes

`take_axis` sets 1 field of the next state of the gamepad for `SDL_EVENT_GAMEPAD_AXIS_MOTION`:

| Axis of SDL3 | Field | Value |
| --- | --- | --- |
| `SDL_GAMEPAD_AXIS_LEFTX` | `left_x` | The value of SDL3 divided by 32767, then clamped to -1 to 1. |
| `SDL_GAMEPAD_AXIS_LEFTY` | `left_y` | The same. |
| `SDL_GAMEPAD_AXIS_RIGHTX` | `right_x` | The same. |
| `SDL_GAMEPAD_AXIS_RIGHTY` | `right_y` | The same. |
| `SDL_GAMEPAD_AXIS_LEFT_TRIGGER` | `left_trigger` | The value of SDL3 divided by 32767, then clamped to 0 to 1. |
| `SDL_GAMEPAD_AXIS_RIGHT_TRIGGER` | `right_trigger` | The same. |

A stick of SDL3 goes from -32768 to 32767, with y to the bottom, and a trigger from 0 to 32767: https://wiki.libsdl.org/SDL3/SDL_GamepadAxis.
The clamp keeps -32768 at -1. An axis needs no room, because only the state changes.

## The buttons

`take_pad_button` spawns an `ideatek:GamepadEvent` for `SDL_EVENT_GAMEPAD_BUTTON_DOWN` and `SDL_EVENT_GAMEPAD_BUTTON_UP`, and sets or clears the bit in the next state:

| Button of SDL3 | Bit |
| --- | --- |
| `SDL_GAMEPAD_BUTTON_SOUTH` | `IDEATEK_GAMEPAD_BUTTON_SOUTH` |
| `SDL_GAMEPAD_BUTTON_EAST` | `IDEATEK_GAMEPAD_BUTTON_EAST` |
| `SDL_GAMEPAD_BUTTON_WEST` | `IDEATEK_GAMEPAD_BUTTON_WEST` |
| `SDL_GAMEPAD_BUTTON_NORTH` | `IDEATEK_GAMEPAD_BUTTON_NORTH` |
| `SDL_GAMEPAD_BUTTON_LEFT_SHOULDER` | `IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER` |
| `SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER` | `IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER` |
| `SDL_GAMEPAD_BUTTON_BACK` | `IDEATEK_GAMEPAD_BUTTON_BACK` |
| `SDL_GAMEPAD_BUTTON_START` | `IDEATEK_GAMEPAD_BUTTON_START` |
| `SDL_GAMEPAD_BUTTON_LEFT_STICK` | `IDEATEK_GAMEPAD_BUTTON_LEFT_STICK` |
| `SDL_GAMEPAD_BUTTON_RIGHT_STICK` | `IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK` |
| `SDL_GAMEPAD_BUTTON_DPAD_UP` | `IDEATEK_GAMEPAD_BUTTON_DPAD_UP` |
| `SDL_GAMEPAD_BUTTON_DPAD_DOWN` | `IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN` |
| `SDL_GAMEPAD_BUTTON_DPAD_LEFT` | `IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT` |
| `SDL_GAMEPAD_BUTTON_DPAD_RIGHT` | `IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT` |
| `SDL_GAMEPAD_BUTTON_GUIDE` | `IDEATEK_GAMEPAD_BUTTON_GUIDE` |

Each other button of SDL3 has no bit and no event: the misc buttons, the paddles and the touchpad. The contract keeps only the standard mapping of the W3C.
A button event needs 1 spawn and 48 bytes.
An event of a gamepad with no entity changes nothing.

## The release

`release_pads` spawns a release event for each button whose bit is 1 in the next state of a gamepad, then clears the bits.
The sticks and the triggers keep their values.

## The branches

| Case | Result |
| --- | --- |
| A stick at 32767, and at -32768. | 1 and -1. |
| A trigger at 32767. | 1. |
| A button of the table goes down, then up. | 2 events, and its bit at 1, then at 0. |
| A button outside the table. | No event, and no bit. |
| An event of a gamepad with no entity. | Nothing. |
| A loss of the focus with 1 button down. | 1 event with `down` at `false`, and the bits at 0. The sticks keep their values. |

## Functions

| Function | Result |
| --- | --- |
| `find_pad` | Gives the gamepad of an id of SDL3, or of an entity, or null. |
| `take_axis` | Sets the field of an axis in the next state. |
| `take_pad_button` | Spawns the event of a button, and keeps its bit. |
| `release_pad` | Spawns the release of each button of 1 gamepad that is down. |
| `release_pads` | Spawns the release of each button that is down. |
