# src/platform_abstraction/input_sdl3/input_take.c

This file takes the events of the input module from the queue of SDL3, in their order, and gives each one to its family.

## The take

`take_events` reads the queue with 1 range, from `SDL_EVENT_KEY_DOWN` to `SDL_EVENT_FINGER_CANCELED`.
The ranges of the module are inside it. Each type that is off never comes into the queue: `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`, *The events that no part takes*.
Thus `order` follows the order of the user across the families.

```pseudo
PROCEDURE take_events()
   DECLARE e : SDL_Event
   DECLARE room : BOOLEAN
   room <- TRUE
   WHILE room AND SDL_PeepEvents(e, 1, SDL_PEEKEVENT, SDL_EVENT_KEY_DOWN, SDL_EVENT_FINGER_CANCELED) = 1
      room <- has_room(need_of(e), bytes_of(e))
      IF room THEN
         CALL SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_EVENT_KEY_DOWN, SDL_EVENT_FINGER_CANCELED)
         CALL take_event(e)
      ENDIF
   ENDWHILE
   CALL flush_run()
ENDPROCEDURE
```

The module first peeks at the event, then takes it only when the round has room for it. Without the room, this event and the ones after it wait in the queue for the next round, in their order.
`need_of` and `bytes_of` give the spawns and the bytes of the event: *The families*. A motion that joins the pending motion needs no room.
`flush_run` attaches the value of the pending run of motions: `docs/intern/platform_abstraction/input_sdl3/input_runs.c.md`.

SDL3 keeps the text of a text event only until the next pump, so the family of the text copies it in the same round.

## The families

`take_event` ends the pending run before each event that is not a motion of the mouse or of a finger. Then it calls the function of the type of the event:

| Types | Function | Document |
| --- | --- | --- |
| `SDL_EVENT_KEY_DOWN`, `SDL_EVENT_KEY_UP` | `take_key` | `docs/intern/platform_abstraction/input_sdl3/input_keys.c.md` |
| `SDL_EVENT_TEXT_INPUT` | `take_text` | `docs/intern/platform_abstraction/input_sdl3/input_text.c.md` |
| `SDL_EVENT_MOUSE_MOTION` | `take_motion` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_MOUSE_BUTTON_DOWN`, `SDL_EVENT_MOUSE_BUTTON_UP` | `take_button` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_MOUSE_WHEEL` | `take_wheel` | `docs/intern/platform_abstraction/input_sdl3/input_mouse.c.md` |
| `SDL_EVENT_GAMEPAD_ADDED` | `add_pad` | `docs/intern/platform_abstraction/input_sdl3/input_gamepads.c.md` |
| `SDL_EVENT_GAMEPAD_REMOVED` | `remove_pad` | `docs/intern/platform_abstraction/input_sdl3/input_gamepads.c.md` |
| `SDL_EVENT_GAMEPAD_AXIS_MOTION` | `take_axis` | `docs/intern/platform_abstraction/input_sdl3/input_pad_state.c.md` |
| `SDL_EVENT_GAMEPAD_BUTTON_DOWN`, `SDL_EVENT_GAMEPAD_BUTTON_UP` | `take_pad_button` | `docs/intern/platform_abstraction/input_sdl3/input_pad_state.c.md` |
| `SDL_EVENT_FINGER_DOWN` to `SDL_EVENT_FINGER_CANCELED` | `take_finger` | `docs/intern/platform_abstraction/input_sdl3/input_fingers.c.md` |
| Each other type of the range | None | The module drops it. |

A type that the module drops has no meaning in the contract.
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
| `need_of` | Gives the spawns that an event needs. |
| `bytes_of` | Gives the bytes of `changes` that an event needs. |
| `take_event` | Calls the function of the family of an event. |
| `take_events` | Takes events until the room of the round ends. |
