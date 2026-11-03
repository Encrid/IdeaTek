# include/input/ideatek_touch.h

`include/input/ideatek_touch.h` gives the types of the touch devices and of the pinches.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The touch devices

A touch device is a device entity with `ideatek:TouchDevice` and `ideatek:TouchState`.
The touchpad of a gamepad is a touch device of `IDEATEK_TOUCH_TYPE_INDIRECT_ABSOLUTE` that names the gamepad.
`x` and `y` of a finger go from 0 to 1 over the device, from the left and the top edges.
On a device of `IDEATEK_TOUCH_TYPE_DIRECT`, they go over the window.

## The fingers

`fingers` of `ideatek:TouchState` is a block in `device_bytes`, with room for `touch_fingers` records.
Each record is an `ideatek_finger_t`. `count` tells how many fingers touch the device.
`ideatek_finger_t` is not a type of Bridge, because no row holds it.
Its descriptor is `ideatek_finger_t_desc`, and its text is `IDEATEK_FINGER_FIELDS`.

## ideatek_finger_t

A record of `fingers` tells 1 finger.
`ideatek_finger_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The finger, while it touches. |
| `window` | `ideatek_entity_t` | 8 | The window of the finger, or 0. |
| `x` | `float` | 16 | The place on x, from 0 to 1. |
| `y` | `float` | 20 | The place on y, from 0 to 1. |
| `pressure` | `float` | 24 | The pressure, from 0 to 1. |

Bytes 28 to 31 hold no field.

## ideatek:TouchDevice

`ideatek:TouchDevice` tells the kind of a touch device.
`ideatek_touch_device_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gamepad` | `ideatek_entity_t` | 0 | The gamepad of a touchpad, or 0. |
| `type` | `ideatek_touch_type_t` | 8 | The kind of the device. |
| `pinch` | `bool` | 9 | `true` if the system gives pinches. |

Bytes 10 to 15 hold no field.

### ideatek:ideatek_touch_type_t

`ideatek_touch_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The device |
| --- | --- | --- |
| `IDEATEK_TOUCH_TYPE_DIRECT` | 0 | A screen: the fingers are on the window. |
| `IDEATEK_TOUCH_TYPE_INDIRECT_ABSOLUTE` | 1 | A touchpad that gives the places of the fingers. |
| `IDEATEK_TOUCH_TYPE_INDIRECT_RELATIVE` | 2 | A touchpad that moves the cursor. |

## ideatek:TouchState

`ideatek:TouchState` holds the fingers on a touch device.
`ideatek_touch_state_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `fingers` | `bridge_text_t` | 0 | The block of the records: *The fingers*. |
| `count` | `uint32_t` | 8 | The number of fingers on the device. |

## ideatek:FingerEvent

`ideatek:FingerEvent` tells a change of a finger.
`ideatek_finger_event_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `finger` | `uint64_t` | 0 | The `id` of the finger. |
| `touch` | `ideatek_entity_t` | 8 | The touch device entity. |
| `window` | `ideatek_entity_t` | 16 | The window of the finger, or 0. |
| `x` | `float` | 24 | The new place on x. |
| `y` | `float` | 28 | The new place on y. |
| `dx` | `float` | 32 | The motion on x, from -1 to 1. |
| `dy` | `float` | 36 | The motion on y, from -1 to 1. |
| `pressure` | `float` | 40 | The new pressure. |
| `order` | `uint32_t` | 44 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `kind` | `ideatek_finger_event_kind_t` | 48 | The change. |

Bytes 49 to 55 hold no field.

### ideatek:ideatek_finger_event_kind_t

`ideatek_finger_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The change |
| --- | --- | --- |
| `IDEATEK_FINGER_EVENT_KIND_DOWN` | 0 | A finger touches the device. |
| `IDEATEK_FINGER_EVENT_KIND_MOTION` | 1 | A finger moves. |
| `IDEATEK_FINGER_EVENT_KIND_UP` | 2 | A finger leaves the device. |
| `IDEATEK_FINGER_EVENT_KIND_CANCELED` | 3 | The system cancels the touch of a finger. |

## ideatek:PinchEvent

`ideatek:PinchEvent` tells a pinch of 2 fingers on a window.
`ideatek_pinch_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window of the pinch. |
| `scale` | `float` | 8 | The scale since the last event: above 1 zooms in. |
| `order` | `uint32_t` | 12 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `kind` | `ideatek_pinch_event_kind_t` | 16 | The step of the pinch. |

Bytes 17 to 23 hold no field.

### ideatek:ideatek_pinch_event_kind_t

`ideatek_pinch_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The step |
| --- | --- | --- |
| `IDEATEK_PINCH_EVENT_KIND_BEGIN` | 0 | The pinch starts. |
| `IDEATEK_PINCH_EVENT_KIND_UPDATE` | 1 | The pinch changes. |
| `IDEATEK_PINCH_EVENT_KIND_END` | 2 | The pinch ends. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_touch_type_t` | `ideatek_touch_type_t` | `ideatek_touch_type_t_desc` | `IDEATEK_TOUCH_TYPE_VALUES` |
| `ideatek:ideatek_finger_event_kind_t` | `ideatek_finger_event_kind_t` | `ideatek_finger_event_kind_t_desc` | `IDEATEK_FINGER_EVENT_KIND_VALUES` |
| `ideatek:ideatek_pinch_event_kind_t` | `ideatek_pinch_event_kind_t` | `ideatek_pinch_event_kind_t_desc` | `IDEATEK_PINCH_EVENT_KIND_VALUES` |
| `ideatek:TouchDevice` | `ideatek_touch_device_t` | `ideatek_touch_device_t_desc` | `IDEATEK_TOUCH_DEVICE_FIELDS` |
| `ideatek:TouchState` | `ideatek_touch_state_t` | `ideatek_touch_state_t_desc` | `IDEATEK_TOUCH_STATE_FIELDS` |
| `ideatek:FingerEvent` | `ideatek_finger_event_t` | `ideatek_finger_event_t_desc` | `IDEATEK_FINGER_EVENT_FIELDS` |
| `ideatek:PinchEvent` | `ideatek_pinch_event_t` | `ideatek_pinch_event_t_desc` | `IDEATEK_PINCH_EVENT_FIELDS` |

```text
{ IDEATEK_TOUCH_TYPE_DIRECT = 0, IDEATEK_TOUCH_TYPE_INDIRECT_ABSOLUTE = 1, IDEATEK_TOUCH_TYPE_INDIRECT_RELATIVE = 2 }
{ IDEATEK_FINGER_EVENT_KIND_DOWN = 0, IDEATEK_FINGER_EVENT_KIND_MOTION = 1, IDEATEK_FINGER_EVENT_KIND_UP = 2, IDEATEK_FINGER_EVENT_KIND_CANCELED = 3 }
{ IDEATEK_PINCH_EVENT_KIND_BEGIN = 0, IDEATEK_PINCH_EVENT_KIND_UPDATE = 1, IDEATEK_PINCH_EVENT_KIND_END = 2 }
{ ideatek_entity_t gamepad; ideatek_touch_type_t type; bool pinch; }
{ bridge_text_t fingers; uint32_t count; }
{ uint64_t finger; ideatek_entity_t touch; ideatek_entity_t window; float x; float y; float dx; float dy; float pressure; uint32_t order; ideatek_finger_event_kind_t kind; }
{ ideatek_entity_t window; float scale; uint32_t order; ideatek_pinch_event_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_touch_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_finger_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pinch_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TouchDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TouchState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FingerEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PinchEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:TouchDevice`, `ideatek:TouchState`, `ideatek:FingerEvent` and `ideatek:PinchEvent` | The field types |
| A reader | No type. | Its types and the field types |
