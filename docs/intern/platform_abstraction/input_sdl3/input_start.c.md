# src/platform_abstraction/input_sdl3/input_start.c

This file reads the settings of the module in its first round, because Bridge writes them after `init`: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_load.

## The first round

`start_input` does these steps, and sets `started` to `true` when they end:

1. It checks `round_entities`. A value outside 1 to 65535 gives `round_entities: <value> is outside 1 to 65535`, and `start_input` gives `false`: `readme.md`, *The contracts*.
2. It sets the hint `SDL_HINT_TOUCH_MOUSE_EVENTS` to `"1"` or `"0"`, from `touch_mouse_events`.
3. If `mappings` is not `"none"`, it calls `SDL_AddGamepadMappingsFromFile` with the path.
4. If SDL3 gives -1, it writes `no mappings <path>: <reason>`. The gamepads keep the mappings of SDL3.

`start_input` copies the text of `mappings` into a C string of the stack, at most 4095 bytes. A longer path gives the line of the mappings.
SDL3 hears each change of that hint with a callback: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_mouse.c#L127. Thus a change in the first round counts for each later touch.
A joystick that was connected before step 3, and that a new mapping makes a gamepad, gets `SDL_EVENT_GAMEPAD_ADDED` then: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/joystick/SDL_gamepad.c#L676.

With `false`, each round gives 1 again, and Bridge stops the module as `on_error` and `max_system_failures` tell.

## The branches

| Case | Result |
| --- | --- |
| `round_entities` of 0 or of 65536. | The system fails after the line of the setting. |
| `touch_mouse_events` at `true`, and at `false`. | A touch gives mouse events, or gives none. |
| `mappings` of `"none"`. | No file is read. |
| `mappings` of a file of mappings. | The gamepads of the file get their mappings. |
| `mappings` of a file that SDL3 cannot read. | The line of the mappings, and the round goes on. |
| A path of `mappings` of 4096 bytes or more. | The line of the mappings. |

## Functions

| Function | Result |
| --- | --- |
| `start_input` | Checks the settings, and gives them to SDL3. Gives `false` for a setting outside its range. |
| `add_mappings` | Adds the file of `mappings`, or writes the line of the mappings. |
