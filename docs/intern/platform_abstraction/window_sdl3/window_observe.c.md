# src/platform_abstraction/window_sdl3/window_observe.c

This file holds the observer `request`, which hears `ideatek:WindowRequest`: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The roles*.

## The observer

`bridge_request` reads the list of its triggers with `bridge_native_get_list` of BB-Native, and gives each trigger to `see_trigger`:

```pseudo
FUNCTION see_trigger(id : U64, events : U32) RETURNS I32
   IF events = BRIDGE_ON_LEAVE THEN
      IF id = window_module.entity THEN
         IF window_module.window <> nullptr THEN
            CALL SDL_HideWindow(window_module.window)
         ENDIF
         RETURN BRIDGE_END
      ENDIF
      RETURN 0
   ENDIF
   IF window_module.entity = 0 THEN
      window_module.entity <- id
   ENDIF
   IF id = window_module.entity THEN
      window_module.pending <- TRUE
   ELSE
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the entity <id> asks for a second window, and gets none")
   ENDIF
   RETURN 0
ENDFUNCTION
```

`bridge_request` gives `BRIDGE_END` if a trigger gave it, else 0.
The observer opens no window and calls no other function of SDL3, because it can run at the barrier of any phase.
Thus the system `events` applies the request in the next first phase, after it read the displays of the round.

The entity of the window leaves the terms when the game destroys it, or when the game detaches its request.
In both cases the module hides the window and ends the run. It destroys the window in `shutdown`.

## The branches

| Case | Result |
| --- | --- |
| The first entity gets `ideatek:WindowRequest`. | The next first phase opens the window. |
| A second entity gets `ideatek:WindowRequest`. | The line of the second window. No window opens. |
| The game destroys the entity of the window, or detaches its request. | The window hides, and the observer gives `BRIDGE_END`. |
| A different entity leaves the terms. | Nothing. |
| The game destroys the entity of the window before the window opens. | The observer gives `BRIDGE_END`. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_request` | Gives each trigger to `see_trigger`. Gives `BRIDGE_END` if the window entity left, else 0. |
| `see_trigger` | Keeps the entity of the window, marks its request, or writes the line of a second window. |
