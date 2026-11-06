# src/platform_abstraction/input_sdl3/input_take.c

This file takes the events of the input module from the queue of SDL3, in their order, and gives each one to its family.

## The take

`take_events` reads the queue with 1 range, from `SDL_EVENT_KEY_DOWN` to `SDL_EVENT_FINGER_CANCELED`.
The ranges of the module are inside it. Each type that is off never comes into the queue: `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`, *The events that no part takes*.
Thus `order` follows the order of the user across the families.

```pseudo
PROCEDURE take_events()
   DECLARE e : SDL_Event
   DECLARE f : input_family_t
   DECLARE room : BOOLEAN
   room <- TRUE
   WHILE room AND SDL_PeepEvents(e, 1, SDL_PEEKEVENT, SDL_EVENT_KEY_DOWN, SDL_EVENT_FINGER_CANCELED) = 1
      f <- family_of(e)
      room <- fits(e, f)
      IF room THEN
         CALL SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_EVENT_KEY_DOWN, SDL_EVENT_FINGER_CANCELED)
         IF NOT is_motion(e) THEN
            CALL flush_run()
         ENDIF
         IF f <> nullptr THEN
            CALL take(f, e)
         ENDIF
      ENDIF
   ENDWHILE
   CALL flush_run()
ENDPROCEDURE
```

The module first peeks at the event, then takes it only when the round has room for it. Without the room, this event and the ones after it wait in the queue for the next round, in their order.
`fits` gives the room of the row of the event: its spawns, and its bytes for each spawn. A text needs 1 spawn for each code point. A motion that joins the pending run, and a type with no row, need no room.
Before each event that is not such a motion, `flush_run` attaches the value of the pending run: `docs/intern/platform_abstraction/input_sdl3/input_runs.c.md`.

SDL3 keeps the text of a text event only until the next pump, so the family of the text copies it in the same round.

## The families

`FAMILIES` holds 1 row for each family, and `family_of` finds the row of the type of an event:

| Types | Spawns | Bytes | Function | Document |
| --- | --- | --- | --- | --- |
| `SDL_EVENT_KEY_DOWN`, `SDL_EVENT_KEY_UP` | 1 | 44 | `take_key` | `docs/intern/platform_abstraction/input_sdl3/input_keys.c.md` |
| `SDL_EVENT_TEXT_INPUT` | 1 for each code point | 32 | `take_text` | `docs/intern/platform_abstraction/input_sdl3/input_text.c.md` |
| `SDL_EVENT_MOUSE_MOTION` | 1 | 48 | `take_motion` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_MOUSE_BUTTON_DOWN`, `SDL_EVENT_MOUSE_BUTTON_UP` | 1 | 40 | `take_button` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_MOUSE_WHEEL` | 1 | 36 | `take_wheel` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_GAMEPAD_AXIS_MOTION` | 0 | 0 | `take_axis` | `docs/intern/platform_abstraction/input_sdl3/input_pad_state.c.md` |
| `SDL_EVENT_GAMEPAD_BUTTON_DOWN`, `SDL_EVENT_GAMEPAD_BUTTON_UP` | 1 | 48 | `take_pad_button` | `docs/intern/platform_abstraction/input_sdl3/input_pad_state.c.md` |
| `SDL_EVENT_GAMEPAD_ADDED` | 1 | 455 | `add_pad` | `docs/intern/platform_abstraction/input_sdl3/input_gamepads.c.md` |
| `SDL_EVENT_GAMEPAD_REMOVED` | 0 | 24 | `remove_pad` | `docs/intern/platform_abstraction/input_sdl3/input_gamepads.c.md` |
| `SDL_EVENT_FINGER_DOWN` to `SDL_EVENT_FINGER_CANCELED` | 1 | 48 | `take_finger` | `docs/intern/platform_abstraction/input_sdl3/input_fingers.c.md` |

Each function of a row takes the API and the event, so 1 table calls each family.
The module drops each other type of the range. Such a type has no meaning in the contract.
Examples are the joysticks and the composition of an IME, which the OS draws. Others are the hot plug of a keyboard or a mouse, and the touchpad and sensors of a gamepad.

## The branches

| Case | Result |
| --- | --- |
| Events of 2 families in a round. | Their ranks follow their order in the queue. |
| An event of a type that the module drops. | It leaves the queue, and gets no entity. |
| More events than the room of a round. | The next round takes the rest, in their order. |

## Functions

| Function | Result |
| --- | --- |
| `family_of` | Gives the row of *The families* of an event, or null. |
| `is_motion` | Tells if an event is a motion that joins the pending run. |
| `fits` | Tells if the round has room for an event. |
| `take_events` | Takes events until the room of the round ends. |
