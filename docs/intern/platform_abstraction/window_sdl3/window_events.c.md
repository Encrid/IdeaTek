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

`wait_for_events` reads `wait` of the `ideatek:EventWait` of the window: `docs/extern/platform_abstraction/ideatek_window.h.md`, *ideatek:EventWait*.

```pseudo
PROCEDURE wait_for_events()
   DECLARE at : U32
   DECLARE left : I64
   at <- 0
   IF window_module.entity <> 0 THEN
      at <- get_cell(window_module.entity, EVENT_WAIT)
   ENDIF
   left <- 0
   IF at <> 0 AND window_module.taken > 0 THEN
      left <- window_module.taken + INT(read_f64(at) * 1000000000) - SDL_GetTicksNS()
   ENDIF
   IF left > 0 THEN
      CALL SDL_WaitEventTimeout(nullptr, (left + 999999) DIV 1000000)
   ELSE
      CALL SDL_PumpEvents()
   ENDIF
ENDPROCEDURE
```

- With no window, or before the window entity has `ideatek:EventWait`, the module does not wait.
- `read_f64` reads the `double` at `at`, in the shared memory. The module reads only types of its layer.
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
The events of the focus are in the range of the window, so the module takes them. The input module reads `focused` of `ideatek:WindowState` in their place.
The input module runs after `events`, in the same phase.

## The ranges of the module

The first start of a subsystem turns off the other types of events: `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`, *The events that no part takes*.
Thus no event stays in the queue for ever, and each wait can last until its end.
`enable_events` turns on each type of the ranges of the window module in `bridge_init`, after the start of the video:

| Types | Range |
| --- | --- |
| The quit | `SDL_EVENT_QUIT` |
| The displays | `SDL_EVENT_DISPLAY_FIRST` to `SDL_EVENT_DISPLAY_LAST` |
| The window | `SDL_EVENT_WINDOW_FIRST` to `SDL_EVENT_WINDOW_LAST` |
| The clipboard | `SDL_EVENT_CLIPBOARD_UPDATE` |
| The drops | `SDL_EVENT_DROP_FILE` to `SDL_EVENT_DROP_POSITION` |

These are the ranges that *The ranges* takes from the queue. `SDL_SetEventEnabled` with `true` turns on 1 type.

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
| `wait` is 0, or the window has no `ideatek:EventWait`. | No wait. |
| `wait` is more than 0, and an event comes. | The wait ends at the event. |
| `wait` is more than 0, and no event comes. | The round starts `wait` after the take of the last round, or less than 1 ms later. |
| The time of the round is more than `wait`. | No wait. |
| An event of the window. | `ideatek:WindowState` is read again. |
| An event of a display. | The displays are read again. |
| A close request, or a quit, with `close_on_request` at `true`. | The window hides, and `bridge_events` gives `BRIDGE_END`. |
| A close request with no window, and `close_on_request` at `true`. | `bridge_events` gives `BRIDGE_END`. |
| A close request with `close_on_request` at `false`. | `close_asked` is `true` in this round, and `false` in the next round. |
| A key press of the keyboard. | It stays in the queue. |
| An event of a type that no part takes. | It never comes into the queue. |
| An event of each range of *The ranges of the module*. | It comes into the queue, and the round takes it. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_events` | Does the work of a round. Gives 0, `BRIDGE_END` after a close request, or 1 with bad settings. |
| `wait_for_events` | Waits at most `wait` for an event, or pumps the queue. |
| `take_events` | Takes the events of the module from the queue. Gives `true` after a close request or a quit. |
| `answer_close` | Hides the window and gives `BRIDGE_END`, or sets `close_asked`. |
| `enable_events` | Turns on each type of event of the ranges of the module. |
