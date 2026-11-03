# include/input/ideatek_input.h

`include/input/ideatek_input.h` gives the events of the focus, and the rules of each input header.
`readme.md`, *The contracts*, tells the rules of each contract.
Only `include/input/ideatek_gamepad.h` includes `include/world/ideatek_world.h`, for `ideatek_entity_t`.
`docs/extern/world/ideatek_world.h.md` describes `ideatek_entity_t`.

| Header | Family | Document |
| --- | --- | --- |
| `include/input/ideatek_input.h` | The focus | This document |
| `include/input/ideatek_keyboard.h` | The keyboard | `docs/extern/input/ideatek_keyboard.h.md` |
| `include/input/ideatek_text.h` | The text input | `docs/extern/input/ideatek_text.h.md` |
| `include/input/ideatek_mouse.h` | The mouse and the cursor | `docs/extern/input/ideatek_mouse.h.md` |
| `include/input/ideatek_gamepad.h` | The gamepads | `docs/extern/input/ideatek_gamepad.h.md` |
| `include/input/ideatek_touch.h` | The fingers | `docs/extern/input/ideatek_touch.h.md` |

## The input entities

At `init`, the input module spawns 1 entity, and attaches `ideatek:Keyboard` and `ideatek:Mouse` to it.
All the keyboards are 1 keyboard, and all the mice are 1 mouse.
Each gamepad is 1 entity: `docs/extern/input/ideatek_gamepad.h.md` tells its rules.
A pen is given as mouse events, and as touch events on a touch screen.

## The events

Each event is an entity that the input module spawns in the round of the event.
The input module destroys each event entity in the next round.
A reader hears each event with an observer of `BRIDGE_ON_ENTER`.
`order` of an event is its rank among all the input events of the round, from 0.
Thus a reader can sort the keys and the texts in the order of the user.

A state type keeps the last values of a device.
The input module attaches a new state only when its bytes change.

## The focus

When the window loses the keyboard focus, the input module releases each key that is down.
For each key, it spawns an `ideatek:KeyEvent` with `down` at `false`.
It also releases each button of the mouse that is down, with an `ideatek:MouseButton`.
Each finger gives an `ideatek:FingerEvent` of `IDEATEK_FINGER_EVENT_KIND_CANCELED`.
Each gamepad button that is down gives an `ideatek:GamepadEvent` with `down` at `false`.
Then it spawns an `ideatek:InputEvent` with `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST`.

## The requests

The game attaches a request to an entity to ask for an action.
When the game attaches the request again, the input module applies each field that changed.

## The roles

- The game writes `ideatek:TextRequest`, `ideatek:MouseRequest` and `ideatek:GamepadRequest`.
- The input module reads these 3 types, and writes each other struct of the part. A deployment has 1 input module.
- A reader reads the types that it needs. The game and the UI are readers.

## The settings of the input module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `merge_motion` | `true` | `true` or `false` | `true` joins the motions of the mouse or of a finger that follow each other into 1 event. |
| `mappings` | `[]` | a list of texts | The lines of gamepad mappings, in the format of SDL_GameControllerDB, that the input module adds. |
| `touch_mouse_events` | `true` | `true` or `false` | `true` also gives a touch as mouse events. |

## ideatek:InputEvent

`ideatek:InputEvent` tells a change of the focus.
`ideatek_input_event_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `order` | `uint32_t` | 0 | The rank of the event: *The events*. |
| `kind` | `ideatek_input_event_kind_t` | 4 | What changed. |

Bytes 5 to 7 hold no field.

### ideatek:ideatek_input_event_kind_t

`ideatek_input_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The change |
| --- | --- | --- |
| `IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED` | 0 | The window got the keyboard focus. |
| `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST` | 1 | The window lost the keyboard focus. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_input_event_kind_t` | `ideatek_input_event_kind_t` | `ideatek_input_event_kind_t_desc` | `IDEATEK_INPUT_EVENT_KIND_VALUES` |
| `ideatek:InputEvent` | `ideatek_input_event_t` | `ideatek_input_event_t_desc` | `IDEATEK_INPUT_EVENT_FIELDS` |

```text
{ IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED = 0, IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST = 1 }
{ uint32_t order; ideatek_input_event_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_input_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

The field types of a header are the enums and the bitmasks of the header.
The field types of `include/input/ideatek_gamepad.h` also hold `ideatek:ideatek_entity_t`.
A module declares the field types before the structs.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:InputEvent` | The field types |
| A reader | No type. | Its types and the field types |
