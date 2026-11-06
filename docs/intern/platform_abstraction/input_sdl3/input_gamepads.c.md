# src/platform_abstraction/input_sdl3/input_gamepads.c

This file gives each gamepad its entity, and plays the rumble of `ideatek:GamepadRequest`: `docs/extern/platform_abstraction/ideatek_input_gamepad.h.md`.

## A new gamepad

`add_pad` does these steps for `SDL_EVENT_GAMEPAD_ADDED`:

1. If the module holds `INPUT_GAMEPADS` gamepads, it writes `more than 16 gamepads: the gamepad <id> gets no entity`.
2. It opens the gamepad with `SDL_OpenGamepad`. If SDL3 refuses, it writes `no gamepad <id>: <reason>`.
3. It spawns the gamepad entity with `spawn_entity`, and attaches `ideatek:Gamepad` with its name, and an `ideatek:GamepadState` of zeros.

The name comes from `SDL_GetGamepadName`. `SDL_utf8strlcpy` copies at most 378 bytes, and cuts at the last whole code point: https://wiki.libsdl.org/SDL3/SDL_utf8strlcpy.
A longer name gives the line of the long name. A gamepad with no name has a name of length 0.
A new gamepad needs 1 spawn and 455 bytes: 24 and 379 bytes for the name, and 24 and 28 bytes for the state.

## The end of a gamepad

`remove_pad` does these steps for `SDL_EVENT_GAMEPAD_REMOVED`:

1. It finds the gamepad by its id of SDL3. A gamepad with no entity changes nothing.
2. It closes the gamepad with `SDL_CloseGamepad`, and destroys its entity.
3. It removes the gamepad from `pads`.

A reader hears the destroy with an observer of `BRIDGE_ON_LEAVE`.

## The rumble

The observer `rumble` hears `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:GamepadRequest`.
For each trigger on a gamepad entity, `play_rumble` reads the request with `bridge_native_find_value`, and calls `SDL_RumbleGamepad`:

- The low frequency motor, the strong motor, gets `strong` times 65535.
- The high frequency motor, the weak motor, gets `weak` times 65535.
- The rumble lasts `duration_ms`.

A force outside 0 to 1 is clamped to 0 or 1, because the request comes from the game.
If SDL3 refuses, it writes `no rumble on the gamepad <entity>: <reason>`. A request on an entity that is not a gamepad changes nothing.
SDL3 stops a rumble after its duration, and a new rumble replaces the one that plays: https://wiki.libsdl.org/SDL3/SDL_RumbleGamepad.

## The branches

| Case | Result |
| --- | --- |
| A gamepad connects. | An entity with its name and a state of zeros. |
| A 17th gamepad. | The line of the gamepads, and no entity. |
| SDL3 does not open a gamepad. | The line of the gamepad, and no entity. |
| A name of more than 378 bytes. | Its first 378 bytes or less, after the line of the long name. |
| A gamepad disconnects. | Its entity goes. |
| A gamepad with no entity disconnects. | Nothing. |
| A rumble on a gamepad. | SDL3 plays it, with the 2 forces of the request. |
| A force of 2, and a force of -1. | 1 and 0. |
| A rumble that SDL3 refuses. | The line of the rumble. |
| A request on another entity. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `name_pad` | Copies the name of a gamepad, or writes the line of the long name. |
| `add_pad` | Opens a gamepad that connects, and spawns its entity. |
| `remove_pad` | Closes a gamepad that disconnects, and destroys its entity. |
| `play_rumble` | Plays the rumble of a request on a gamepad entity. |
| `bridge_rumble` | Plays the rumble of each trigger. |
