# src/platform_abstraction/input_sdl3/input_main.c

This file holds the 3 entry points of the module that each module has: `init`, `shutdown` and `reload`.
It also holds the variable `input_module`: `docs/intern/platform_abstraction/input_sdl3/input_inside.h.md`, *The state of the module*.

## The start

`bridge_init` does these steps:

1. It starts the video and the gamepads of SDL3 with `sdl3_start_subsystem` and `SDL_INIT_VIDEO | SDL_INIT_GAMEPAD`: `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`.
2. If SDL3 refuses, it writes `no input: <reason>`, and gives 1. The load fails.
3. It turns on the types of the events of the module with `enable_events`.
4. It declares the types with `declare_types`: `docs/intern/platform_abstraction/input_sdl3/input_declare.c.md`.
5. It adds the system and the 4 observers with `add_runs`.
6. It spawns the input entity, and attaches a `ideatek:Keyboard` and a `ideatek:Mouse` of zeros to it.
7. It gives 0.

The video holds the keyboard, the mouse, the text input and the fingers. The window module holds the video too, and `sdl3_start` counts each hold.

## The types of the events

`enable_events` turns on the ranges that `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`, *The events that no part takes*, gives to the input module.

| First type | Last type | Family |
| --- | --- | --- |
| `SDL_EVENT_KEY_DOWN` | `SDL_EVENT_SCREEN_KEYBOARD_HIDDEN` | The keyboard and the text input. |
| `SDL_EVENT_MOUSE_MOTION` | `SDL_EVENT_MOUSE_REMOVED` | The mouse. |
| `SDL_EVENT_JOYSTICK_AXIS_MOTION` | `SDL_EVENT_JOYSTICK_UPDATE_COMPLETE` | The joysticks. |
| `SDL_EVENT_GAMEPAD_AXIS_MOTION` | `SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED` | The gamepads. |
| `SDL_EVENT_FINGER_DOWN` | `SDL_EVENT_FINGER_CANCELED` | The fingers. |

The module takes each event of these ranges, also a type that it does not use: `docs/intern/platform_abstraction/input_sdl3/input_take.c.md`.
Thus no event of these ranges stays in the queue, and a wait of the window module ends only at a new event.

## The runs

`add_runs` adds:

- The system `events`, in the phase `first`, with no terms. It changes `ideatek:InputEvent`, the state types and the event types of the input headers: `docs/intern/platform_abstraction/input_sdl3/input_events.c.md`.
- The observer `window`, on `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:WindowState`: `docs/intern/platform_abstraction/input_sdl3/input_events.c.md`, *The focus*.
- The observer `text`, on `BRIDGE_ON_ENTER`, `BRIDGE_ON_ATTACH` and `BRIDGE_ON_LEAVE` of `ideatek:TextRequest`: `docs/intern/platform_abstraction/input_sdl3/input_text.c.md`.
- The observer `mouse`, on `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:MouseRequest`: `docs/intern/platform_abstraction/input_sdl3/input_cursor.c.md`.
- The observer `rumble`, on `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:GamepadRequest`: `docs/intern/platform_abstraction/input_sdl3/input_gamepads.c.md`.

It keeps the index of the list of each observer in `input_module`.

## The end

`bridge_shutdown` does these steps:

1. It stops the text input, if it runs.
2. It closes each gamepad with `SDL_CloseGamepad`.
3. It destroys each cursor that the module made with `SDL_DestroyCursor`.
4. It stops the video and the gamepads with `sdl3_stop_subsystem`, and gives 0.

No row of the module is saved: each row is a fact of the devices, and the next start reads it again.
`bridge_reload` gives 0: Bridge never reloads this module, and a reload has nothing to do.

## The branches

| Case | Result |
| --- | --- |
| SDL3 does not start the video or the gamepads. | The line of no input, and the load fails. |
| SDL3 starts them. | The load gives 0. The input entity has `ideatek:Keyboard` and `ideatek:Mouse`. |
| A reload. | It gives 0. |
| The end of a run with the text input on, 1 gamepad and 1 cursor. | Each goes, then the video and the gamepads of SDL3. |
| The end of a run with no device. | The video and the gamepads of SDL3 go. |

## Functions

| Function | Result |
| --- | --- |
| `add_runs` | Adds the system `events` and the 4 observers. |
| `enable_events` | Turns on the types of the events of the module. |
| `bridge_init` | Starts SDL3, declares the types, adds the runs and spawns the input entity. |
| `bridge_shutdown` | Stops the text input, closes the gamepads and the cursors, and stops SDL3. |
| `bridge_reload` | Gives 1. |
