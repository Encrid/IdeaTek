# src/platform_abstraction/input_sdl3/input_events.c

This file holds the system `events`, which does the work of the input module in each round, and the observer `window`, which reads the focus.

## The round

`bridge_events` runs in the phase `first`, after the system `events` of the window module:

```pseudo
FUNCTION bridge_events() RETURNS I32
   IF NOT input_module.started AND NOT start_input() THEN
      RETURN 1
   ENDIF
   CALL turn_items()
   IF input_module.focused AND NOT input_module.was_focused THEN
      CALL spawn_focus(IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED)
   ENDIF
   CALL take_events()
   IF NOT input_module.was_focused OR input_module.focused OR release_all() THEN
      input_module.was_focused <- input_module.focused
   ENDIF
   CALL attach_states()
   RETURN 0
ENDFUNCTION
```

`start_input` reads the settings in the first round: `docs/intern/platform_abstraction/input_sdl3/input_start.c.md`.
`turn_items` clears the events of the last round: `docs/intern/platform_abstraction/input_sdl3/input_items.c.md`.
`take_events` takes the events of SDL3, in their order: `docs/intern/platform_abstraction/input_sdl3/input_take.c.md`.
`release_all` releases each key, button and finger that is down, then gives the loss of the focus: `docs/intern/platform_abstraction/input_sdl3/input_focus.c.md`.
`attach_states` attaches each state whose bytes changed: *The states*.

The gain of the focus comes before the events of the round, and the loss after them. Thus a key that goes down before a loss still gets its event, then its release.

## The focus

The observer `window` hears `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:WindowState`.
For each trigger, it reads `focused` of the entity with `bridge_native_find_value`, and keeps it in `input_module.focused`: https://github.com/Encrid/BB-Native/blob/v0.5.1/docs/extern/bridge_native_value.h.md.
The window module attaches its state in the same phase, and Bridge applies it at the barrier after the phase. Thus the round reads the focus of the round before.
With no window, `focused` stays `false`, and the module gives no event of the focus.

## The states

`attach_states` compares the value of each state of the round with the last value that the module attached:

- `ideatek:Keyboard` and `ideatek:Mouse` of the input entity.
- `ideatek:GamepadState` of each gamepad entity.

If the bytes differ, and the round has room in `changes`, it attaches the new value, and keeps it as the last value.
Without the room, the next round attaches it.

## The branches

| Case | Result |
| --- | --- |
| The first round, with a setting outside its range. | `bridge_events` gives 1. |
| The window gets the focus. | An `ideatek:InputEvent` of `IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED`, before the events of the round. |
| The window loses the focus. | The releases, then an `ideatek:InputEvent` of `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST`, after the events of the round. |
| No change of the focus. | No event of the focus. |
| A round that changes a state. | 1 attach of that state. |
| A round that changes no state. | No attach. |
| No room in `changes` for a state. | The next round attaches it. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_events` | Does the work of the round. |
| `bridge_window` | Reads the focus of each trigger of `ideatek:WindowState`. |
| `take_change` | Copies a new value of a state, if its bytes changed and the round has room. |
| `attach_states` | Attaches each state whose bytes changed. |
