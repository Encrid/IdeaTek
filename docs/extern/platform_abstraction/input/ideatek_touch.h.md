# include/platform_abstraction/input/ideatek_touch.h

`include/platform_abstraction/input/ideatek_touch.h` gives the types of the fingers.
`docs/extern/platform_abstraction/input/ideatek_input.h.md` tells the common rules.

## The fingers

Each finger on a touch screen gives an `ideatek:FingerEvent` when it touches, moves or leaves the screen.
`id` stays the same while the finger touches.
With `merge_motion`, the motions of 1 finger that follow each other join into 1 event.
With `touch_mouse_events`, the input module also gives a touch as mouse events.

## ideatek:FingerEvent

`ideatek:FingerEvent` tells a change of a finger.
`ideatek_finger_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The finger, while it touches. |
| `x` | `float` | 8 | The place of the finger on x in the window, in pixels. |
| `y` | `float` | 12 | The place of the finger on y in the window, in pixels. |
| `order` | `uint32_t` | 16 | The rank of the event: `docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The events*. |
| `kind` | `ideatek_finger_event_kind_t` | 20 | The change. |

Bytes 21 to 23 hold no field.

### ideatek:ideatek_finger_event_kind_t

`ideatek_finger_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The change |
| --- | --- | --- |
| `IDEATEK_FINGER_EVENT_KIND_BEGAN` | 0 | A finger touches the screen. |
| `IDEATEK_FINGER_EVENT_KIND_MOVED` | 1 | A finger moves. |
| `IDEATEK_FINGER_EVENT_KIND_ENDED` | 2 | A finger leaves the screen. |
| `IDEATEK_FINGER_EVENT_KIND_CANCELED` | 3 | The system cancels the touch of a finger. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_finger_event_kind_t` | `ideatek_finger_event_kind_t` | `ideatek_finger_event_kind_t_desc` | `IDEATEK_FINGER_EVENT_KIND_VALUES` |
| `ideatek:FingerEvent` | `ideatek_finger_event_t` | `ideatek_finger_event_t_desc` | `IDEATEK_FINGER_EVENT_FIELDS` |

```text
{ IDEATEK_FINGER_EVENT_KIND_BEGAN = 0, IDEATEK_FINGER_EVENT_KIND_MOVED = 1, IDEATEK_FINGER_EVENT_KIND_ENDED = 2, IDEATEK_FINGER_EVENT_KIND_CANCELED = 3 }
{ uint64_t id; float x; float y; uint32_t order; ideatek_finger_event_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_finger_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FingerEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:FingerEvent` | The field types |
| A reader | No type. | Its types and the field types |
