# src/platform_abstraction/sdl3_start/sdl3_start.c

This file starts and stops the subsystems of SDL3 for the modules: `docs/intern/platform_abstraction/sdl3_start/sdl3_start.h.md`.

## The signals

The signals belong to the host: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The life cycle*.
SDL3 installs its handlers of `SIGINT` and `SIGTERM` when its events start, if the hint `SDL_HINT_NO_SIGNAL_HANDLERS` is not `"1"`.
Thus `start_events` sets the hint while the events start:

```pseudo
FUNCTION start_events() RETURNS BOOLEAN
   DECLARE started : BOOLEAN
   CALL SDL_SetHintWithPriority(SDL_HINT_NO_SIGNAL_HANDLERS, "1", SDL_HINT_OVERRIDE)
   started <- SDL_InitSubSystem(SDL_INIT_EVENTS)
   CALL SDL_ResetHint(SDL_HINT_NO_SIGNAL_HANDLERS)
   RETURN started
ENDFUNCTION
```

The priority `SDL_HINT_OVERRIDE` wins over the variable `SDL_NO_SIGNAL_HANDLERS` of the environment.
Thus the environment cannot give the signals to SDL3.

The reset comes before the start of the other subsystems.
On Linux, the video of the console reads the same hint, and installs handlers that give the keyboard back to the console after a crash.
Those handlers do not replace a handler of `SIGHUP` that the host installed: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/core/linux/SDL_evdev_kbd.c#L292.

## The start

`sdl3_start_subsystem` starts the events first, and then the subsystems of `flags`:

```pseudo
FUNCTION sdl3_start_subsystem(flags : U32, BYREF reason : STRING, size : U64) RETURNS BOOLEAN
   DECLARE started : BOOLEAN
   IF SDL_WasInit(0) = 0 THEN
      CALL disable_events()
   ENDIF
   IF NOT start_events() THEN
      CALL SDL_strlcpy(reason, SDL_GetError(), size)
      RETURN FALSE
   ENDIF
   IF flags = SDL_INIT_EVENTS THEN
      RETURN TRUE
   ENDIF
   started <- SDL_InitSubSystem(flags)
   IF NOT started THEN
      CALL SDL_strlcpy(reason, SDL_GetError(), size)
   ENDIF
   CALL SDL_QuitSubSystem(SDL_INIT_EVENTS)
   RETURN started
ENDFUNCTION
```

Each subsystem of SDL3 other than the events holds the events from its start to its stop.
Thus the start stops its own hold of the events, and the stop of `flags` stops the rest.
With `flags` of `SDL_INIT_EVENTS` alone, the hold of the events is the hold of the caller.

The copy of the reason comes before `SDL_QuitSubSystem`, because a stop can change the text of `SDL_GetError`.
SDL3 keeps its error with the same care: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/SDL.c#L411.
`SDL_strlcpy` cuts a long reason to `size` minus 1 bytes, and always ends it with a zero.

## The events that no part takes

An event that stays in the queue ends each wait of the window module at once, because `SDL_WaitEventTimeout` returns when the queue holds an event.
SDL3 also refuses a new event when the queue holds 65535 events: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_events.c#L54.
Thus at the first start, `disable_events` turns off each type of `SDL_EventType` that no part of the project takes.
The first start is a start while `SDL_WasInit(0)` is 0. It comes before `SDL_InitSubSystem`, so no event of a type that is off comes into the queue.

These are the ranges that a module takes. Each other type of `SDL_EventType` of SDL3 3.4.20 is off:

| Module | Ranges |
| --- | --- |
| The window module | `SDL_EVENT_QUIT`, `SDL_EVENT_DISPLAY_FIRST` to `SDL_EVENT_DISPLAY_LAST`, `SDL_EVENT_WINDOW_FIRST` to `SDL_EVENT_WINDOW_LAST`, `SDL_EVENT_CLIPBOARD_UPDATE`, and `SDL_EVENT_DROP_FILE` to `SDL_EVENT_DROP_POSITION` |
| The input module | `SDL_EVENT_KEY_DOWN` to `SDL_EVENT_SCREEN_KEYBOARD_HIDDEN`, `SDL_EVENT_MOUSE_MOTION` to `SDL_EVENT_MOUSE_REMOVED`, `SDL_EVENT_JOYSTICK_AXIS_MOTION` to `SDL_EVENT_JOYSTICK_UPDATE_COMPLETE`, `SDL_EVENT_GAMEPAD_AXIS_MOTION` to `SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED`, and `SDL_EVENT_FINGER_DOWN` to `SDL_EVENT_FINGER_CANCELED` |

`disable_events` calls `SDL_SetEventEnabled` with `false` for each type of these ranges, which are off:

| Types | Range |
| --- | --- |
| The application, but the quit | `SDL_EVENT_TERMINATING` to `SDL_EVENT_SYSTEM_THEME_CHANGED` |
| The gestures of a pinch | `SDL_EVENT_PINCH_BEGIN` to `SDL_EVENT_PINCH_END` |
| The audio devices | `SDL_EVENT_AUDIO_DEVICE_ADDED` to `SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED` |
| The sensors | `SDL_EVENT_SENSOR_UPDATE` |
| The pens | `SDL_EVENT_PEN_PROXIMITY_IN` to `SDL_EVENT_PEN_AXIS` |
| The cameras | `SDL_EVENT_CAMERA_DEVICE_ADDED` to `SDL_EVENT_CAMERA_DEVICE_DENIED` |
| The renderers of SDL3 | `SDL_EVENT_RENDER_TARGETS_RESET` to `SDL_EVENT_RENDER_DEVICE_LOST` |
| The private platforms | `SDL_EVENT_PRIVATE0` to `SDL_EVENT_PRIVATE3` |
| The events of a program | `SDL_EVENT_USER` to `SDL_EVENT_LAST` |

`SDL_EVENT_POLL_SENTINEL` stays on. It is a mark of SDL3 in its own queue, and `SDL_PeepEvents` never gives it to a module.
The input module takes the joystick range, because SDL3 turns it on with the types of the gamepads: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_events.c#L1911.

Each module turns on the types of its ranges after its start, because the 2 modules share 1 state of SDL3.
SDL3 forgets each type that is off when its events stop: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_events.c#L985.
Thus a start after the stop of SDL3 is a first start again, and turns the types off again.

## The stop

`sdl3_stop_subsystem` stops the subsystems of `flags`:

```pseudo
PROCEDURE sdl3_stop_subsystem(flags : U32)
   CALL SDL_QuitSubSystem(flags)
   IF SDL_WasInit(0) = 0 THEN
      CALL SDL_Quit()
   ENDIF
ENDPROCEDURE
```

`SDL_WasInit(0)` gives the subsystems that a module still holds. Thus the last module that stops also stops SDL3.

## The branches

Each case of this table is a case of Reflection:

| Case | Result |
| --- | --- |
| The first start. | Each type of the table of *The events that no part takes* is off. |
| A start while a different module holds a subsystem. | No type changes. |
| The events do not start. | `false`, and the reason of SDL3 is in `reason`. No subsystem stays. |
| `flags` is `SDL_INIT_EVENTS`, and the events start. | `true`. The caller holds the events. |
| The subsystems of `flags` do not start. | `false`, the reason is in `reason`, and the hold of the events stops. |
| The subsystems of `flags` start. | `true`. The caller holds `flags`. |
| The stop of the last subsystem. | `SDL_Quit` runs. |
| The stop while a different module holds a subsystem. | `SDL_Quit` does not run. |

## Functions

| Function | Result |
| --- | --- |
| `disable_events` | Turns off each type of event that no part takes. |
| `start_events` | Starts the events of SDL3 with no handler for the signals. Gives the result of `SDL_InitSubSystem`. |
| `sdl3_start_subsystem` | Starts the events, then the subsystems of `flags`. Gives `true`, or `false` with the reason. |
| `sdl3_stop_subsystem` | Stops the subsystems of `flags`, then SDL3 after the last subsystem. |
