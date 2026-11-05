# src/platform_abstraction/window_sdl3/window_events.c

This file holds the system `events`, which runs in the first phase, and takes the events of SDL3 for the window module.
It is the only code of the modules of SDL3 that pumps the queue of SDL3.

## A round

`bridge_events` does the work of a round in this order:

```pseudo
FUNCTION bridge_events() RETURNS I32
   DECLARE asked : BOOLEAN
   DECLARE status : I32
   IF window_module.drops.at = 0 THEN
      IF NOT start_items() THEN
         RETURN 1
      ENDIF
   ENDIF
   CALL wait_for_events()
   CALL turn_items()
   asked <- take_events()
   window_module.taken <- SDL_GetTicksNS()
   IF window_module.displays_dirty THEN
      CALL sync_displays()
   ENDIF
   IF window_module.pending OR window_module.moved THEN
      CALL apply_request()
   ENDIF
   CALL spawn_pastes()
   status <- answer_close(asked)
   CALL attach_state()
   RETURN status
ENDFUNCTION
```

- `start_items` runs in the first round only: `docs/intern/platform_abstraction/window_sdl3/window_items.c.md`, *The first round*.
- `turn_items` comes before the new items, so the items of the last round go first: `docs/intern/platform_abstraction/window_sdl3/window_items.c.md`, *A turn*.
- The displays come before the request, so that a request can name a display of this round.
- Each step that spawns or attaches checks the room of the round first: `docs/intern/platform_abstraction/window_sdl3/window_world.c.md`, *The room of a round*.

## The wait

`wait_for_events` reads `wait` of `ideatek:Frame`: `docs/extern/kernel/ideatek_frame.h.md`, *The limit of the rate*.

```pseudo
PROCEDURE wait_for_events()
   DECLARE frame : ideatek_frame_t
   DECLARE left : I64
   frame <- bridge_native_get_single(FRAME)
   left <- 0
   IF frame <> nullptr AND window_module.taken > 0 THEN
      left <- window_module.taken + INT(frame.wait * 1000000000) - SDL_GetTicksNS()
   ENDIF
   IF left > 0 THEN
      CALL SDL_WaitEventTimeout(nullptr, (left + 999999) DIV 1000000)
   ELSE
      CALL SDL_PumpEvents()
   ENDIF
ENDPROCEDURE
```

- With no clock module, `ideatek:Frame` has no value, and the module does not wait.
- The first round does not wait, because `taken` is 0 before it.
- `SDL_WaitEventTimeout` takes a time in milliseconds. The module rounds the time up, so a round never comes before `wait` passed.
- With no buffer for the event, `SDL_WaitEventTimeout` takes no event from the queue. It pumps the queue before and after the wait.

## The ranges

`take_events` takes the events of the window module from the queue, and leaves each other event there:

```pseudo
FUNCTION take_events() RETURNS BOOLEAN
   DECLARE asked : BOOLEAN
   CALL count_pastes()
   asked <- SDL_HasEvent(SDL_EVENT_QUIT) OR SDL_HasEvent(SDL_EVENT_WINDOW_CLOSE_REQUESTED)
   IF SDL_HasEvents(SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST) THEN
      window_module.dirty <- TRUE
   ENDIF
   IF SDL_HasEvents(SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST) THEN
      window_module.displays_dirty <- TRUE
   ENDIF
   CALL SDL_FlushEvents(SDL_EVENT_QUIT, SDL_EVENT_QUIT)
   CALL SDL_FlushEvents(SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST)
   CALL SDL_FlushEvents(SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST)
   CALL SDL_FlushEvents(SDL_EVENT_CLIPBOARD_UPDATE, SDL_EVENT_CLIPBOARD_UPDATE)
   CALL take_drops()
   RETURN asked
ENDFUNCTION
```

The events of the window and of the displays carry no value that the module keeps.
The module reads the window and the displays from SDL3 after the events, so 1 test of each range is enough.
Only the events of the drops carry text, so `take_drops` takes them 1 at a time: `docs/intern/platform_abstraction/window_sdl3/window_drops.c.md`.

`count_pastes` reads the key presses before the input module takes them, and takes none: `docs/intern/platform_abstraction/window_sdl3/window_clipboard.c.md`, *A paste*.
The events of the keyboard, the mouse, the text, the touch and the gamepads stay in the queue for the input module.
The input module runs after `events`, in the same phase.

## The events that no module takes

An event that stays in the queue ends each wait at once, because `SDL_WaitEventTimeout` returns when the queue holds an event.
SDL3 also refuses a new event when the queue holds 65535 events: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_events.c#L54.
Thus `disable_events` turns off each type that no part of the project takes, in `bridge_init`, after the start of the video:

| Types | Range |
| --- | --- |
| The events of the application, but the quit: the end, the memory, the background, the foreground, the locale and the theme. | `SDL_EVENT_TERMINATING` to `SDL_EVENT_SYSTEM_THEME_CHANGED` |
| The audio devices. | `SDL_EVENT_AUDIO_DEVICE_ADDED` to `SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED` |
| The sensors. | `SDL_EVENT_SENSOR_UPDATE` |
| The pens. | `SDL_EVENT_PEN_PROXIMITY_IN` to `SDL_EVENT_PEN_AXIS` |
| The cameras. | `SDL_EVENT_CAMERA_DEVICE_ADDED` to `SDL_EVENT_CAMERA_DEVICE_DENIED` |
| The renderers of SDL3. | `SDL_EVENT_RENDER_TARGETS_RESET` to `SDL_EVENT_RENDER_DEVICE_LOST` |

`SDL_SetEventEnabled` turns off 1 type, and SDL3 then drops each event of that type before the queue.
A part that takes a type of this table turns it on again in its `init`, after the window module loaded.

## The close

`answer_close` answers a close request: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The life cycle*.

```pseudo
FUNCTION answer_close(asked : BOOLEAN) RETURNS I32
   DECLARE settings : window_settings_t
   settings <- bridge_native_get_single(SETTINGS)
   IF asked AND settings.close_on_request THEN
      IF window_module.window <> nullptr THEN
         CALL SDL_HideWindow(window_module.window)
      ENDIF
      RETURN BRIDGE_END
   ENDIF
   IF asked <> window_module.close_asked THEN
      window_module.close_asked <- asked
      window_module.dirty <- TRUE
   ENDIF
   RETURN 0
ENDFUNCTION
```

The module hides the window, and destroys it only in `shutdown`.
Bridge ends the run after the round, so the modules of the later phases can still use the handles of the window.
A close request and a quit in 1 round are 1 request. SDL3 sends both when the user closes the last window.

## The branches

| Case | Result |
| --- | --- |
| The first round, with settings out of their range. | `bridge_events` gives 1. |
| `wait` is 0, or no clock module runs. | No wait. |
| `wait` is more than 0, and an event comes. | The wait ends at the event. |
| `wait` is more than 0, and no event comes. | The round starts `wait` after the take of the last round, or less than 1 ms later. |
| The time of the round is more than `wait`. | No wait. |
| An event of the window. | `ideatek:WindowState` is read again. |
| An event of a display. | The displays are read again. |
| A close request, or a quit, with `close_on_request` at `true`. | The window hides, and `bridge_events` gives `BRIDGE_END`. |
| A close request with no window, and `close_on_request` at `true`. | `bridge_events` gives `BRIDGE_END`. |
| A close request with `close_on_request` at `false`. | `close_asked` is `true` in this round, and `false` in the next round. |
| A key press of the keyboard. | It stays in the queue. |
| An event of a type of *The events that no module takes*. | It never comes into the queue. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_events` | Does the work of a round. Gives 0, `BRIDGE_END` after a close request, or 1 with bad settings. |
| `wait_for_events` | Waits for an event for at most `wait`, or pumps the queue. |
| `take_events` | Takes the events of the module from the queue. Gives `true` after a close request or a quit. |
| `answer_close` | Hides the window and gives `BRIDGE_END`, or sets `close_asked`. |
| `disable_events` | Turns off each type of event that no part takes. |
