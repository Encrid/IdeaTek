# include/input/ideatek_pen.h

`include/input/ideatek_pen.h` gives the types of the pens.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The pens

A pen is a device entity with `ideatek:Pen` and `ideatek:PenState`.
A pen can also give mouse events and touch events: `docs/extern/input/ideatek_input.h.md`, *The settings of the input module*.
The buttons of a pen go from 1 to 5.

## ideatek:Pen

`ideatek:Pen` tells the kind of a pen.
`ideatek_pen_t` takes 1 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `type` | `ideatek_pen_type_t` | 0 | The kind of the pen. |

### ideatek:ideatek_pen_type_t

`ideatek_pen_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The pen |
| --- | --- | --- |
| `IDEATEK_PEN_TYPE_UNKNOWN` | 0 | Not known. |
| `IDEATEK_PEN_TYPE_DIRECT` | 1 | The pen touches the screen. |
| `IDEATEK_PEN_TYPE_INDIRECT` | 2 | The pen touches a tablet. |

## ideatek:PenState

`ideatek:PenState` holds the state of a pen.
`ideatek_pen_state_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window under the pen, or 0. |
| `x` | `float` | 8 | The place on x in the window, in pixels. |
| `y` | `float` | 12 | The place on y in the window, in pixels. |
| `axes` | `float[7]` | 16 | The value of each axis. |
| `flags` | `ideatek_pen_input_t` | 44 | The flags of the pen. |

### ideatek:ideatek_pen_axis_t

`ideatek_pen_axis_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The axis |
| --- | --- | --- |
| `IDEATEK_PEN_AXIS_PRESSURE` | 0 | The pressure, from 0 to 1. |
| `IDEATEK_PEN_AXIS_XTILT` | 1 | The tilt from the left to the right, from -90 to 90 degrees. |
| `IDEATEK_PEN_AXIS_YTILT` | 2 | The tilt from the top to the bottom, from -90 to 90 degrees. |
| `IDEATEK_PEN_AXIS_DISTANCE` | 3 | The distance to the surface, from 0 to 1. |
| `IDEATEK_PEN_AXIS_ROTATION` | 4 | The turn of the barrel, clockwise, from -180 to 180 degrees. |
| `IDEATEK_PEN_AXIS_SLIDER` | 5 | The finger wheel or the slider, from 0 to 1. |
| `IDEATEK_PEN_AXIS_TANGENTIAL_PRESSURE` | 6 | The pressure of a squeeze of the barrel, from 0 to 1. |

### ideatek:ideatek_pen_input_t

`ideatek_pen_input_t` is a bitmask of `uint32_t`. Its size and its alignment are 4.

| Constant | Value | The flag |
| --- | --- | --- |
| `IDEATEK_PEN_INPUT_DOWN` | 0x00000001 | The tip touches the surface. |
| `IDEATEK_PEN_INPUT_BUTTON_1` | 0x00000002 | The button 1 is down. |
| `IDEATEK_PEN_INPUT_BUTTON_2` | 0x00000004 | The button 2 is down. |
| `IDEATEK_PEN_INPUT_BUTTON_3` | 0x00000008 | The button 3 is down. |
| `IDEATEK_PEN_INPUT_BUTTON_4` | 0x00000010 | The button 4 is down. |
| `IDEATEK_PEN_INPUT_BUTTON_5` | 0x00000020 | The button 5 is down. |
| `IDEATEK_PEN_INPUT_ERASER_TIP` | 0x40000000 | The tip is the eraser. |
| `IDEATEK_PEN_INPUT_IN_PROXIMITY` | 0x80000000 | The pen is near the surface. |

## ideatek:PenEvent

`ideatek:PenEvent` tells a change of a pen.
`ideatek_pen_event_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `pen` | `ideatek_entity_t` | 0 | The pen entity. |
| `window` | `ideatek_entity_t` | 8 | The window under the pen, or 0. |
| `x` | `float` | 16 | The place on x, in pixels. |
| `y` | `float` | 20 | The place on y, in pixels. |
| `value` | `float` | 24 | With `IDEATEK_PEN_EVENT_KIND_AXIS`: the new value. |
| `flags` | `ideatek_pen_input_t` | 28 | The flags at the time of the event. |
| `order` | `uint32_t` | 32 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `button` | `uint8_t` | 36 | With a kind of a button: the button. |
| `axis` | `ideatek_pen_axis_t` | 37 | With `IDEATEK_PEN_EVENT_KIND_AXIS`: the axis. |
| `kind` | `ideatek_pen_event_kind_t` | 38 | The change. |
| `eraser` | `bool` | 39 | With a kind of the tip: `true` if the tip is the eraser. |

### ideatek:ideatek_pen_event_kind_t

`ideatek_pen_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The change |
| --- | --- | --- |
| `IDEATEK_PEN_EVENT_KIND_PROXIMITY_IN` | 0 | The pen comes near the surface. |
| `IDEATEK_PEN_EVENT_KIND_PROXIMITY_OUT` | 1 | The pen goes away from the surface. |
| `IDEATEK_PEN_EVENT_KIND_TIP_DOWN` | 2 | The tip touches the surface. |
| `IDEATEK_PEN_EVENT_KIND_TIP_UP` | 3 | The tip leaves the surface. |
| `IDEATEK_PEN_EVENT_KIND_BUTTON_DOWN` | 4 | A button goes down. |
| `IDEATEK_PEN_EVENT_KIND_BUTTON_UP` | 5 | A button goes up. |
| `IDEATEK_PEN_EVENT_KIND_MOTION` | 6 | The pen moves. |
| `IDEATEK_PEN_EVENT_KIND_AXIS` | 7 | An axis changes. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_pen_type_t` | `ideatek_pen_type_t` | `ideatek_pen_type_t_desc` | `IDEATEK_PEN_TYPE_VALUES` |
| `ideatek:ideatek_pen_axis_t` | `ideatek_pen_axis_t` | `ideatek_pen_axis_t_desc` | `IDEATEK_PEN_AXIS_VALUES` |
| `ideatek:ideatek_pen_input_t` | `ideatek_pen_input_t` | `ideatek_pen_input_t_desc` | `IDEATEK_PEN_INPUT_VALUES` |
| `ideatek:ideatek_pen_event_kind_t` | `ideatek_pen_event_kind_t` | `ideatek_pen_event_kind_t_desc` | `IDEATEK_PEN_EVENT_KIND_VALUES` |
| `ideatek:Pen` | `ideatek_pen_t` | `ideatek_pen_t_desc` | `IDEATEK_PEN_FIELDS` |
| `ideatek:PenState` | `ideatek_pen_state_t` | `ideatek_pen_state_t_desc` | `IDEATEK_PEN_STATE_FIELDS` |
| `ideatek:PenEvent` | `ideatek_pen_event_t` | `ideatek_pen_event_t_desc` | `IDEATEK_PEN_EVENT_FIELDS` |

```text
{ IDEATEK_PEN_TYPE_UNKNOWN = 0, IDEATEK_PEN_TYPE_DIRECT = 1, IDEATEK_PEN_TYPE_INDIRECT = 2 }
{ IDEATEK_PEN_AXIS_PRESSURE = 0, IDEATEK_PEN_AXIS_XTILT = 1, IDEATEK_PEN_AXIS_YTILT = 2, IDEATEK_PEN_AXIS_DISTANCE = 3, IDEATEK_PEN_AXIS_ROTATION = 4, IDEATEK_PEN_AXIS_SLIDER = 5, IDEATEK_PEN_AXIS_TANGENTIAL_PRESSURE = 6 }
{ IDEATEK_PEN_INPUT_DOWN = 0x00000001, IDEATEK_PEN_INPUT_BUTTON_1 = 0x00000002, IDEATEK_PEN_INPUT_BUTTON_2 = 0x00000004, IDEATEK_PEN_INPUT_BUTTON_3 = 0x00000008, IDEATEK_PEN_INPUT_BUTTON_4 = 0x00000010, IDEATEK_PEN_INPUT_BUTTON_5 = 0x00000020, IDEATEK_PEN_INPUT_ERASER_TIP = 0x40000000, IDEATEK_PEN_INPUT_IN_PROXIMITY = 0x80000000 }
{ IDEATEK_PEN_EVENT_KIND_PROXIMITY_IN = 0, IDEATEK_PEN_EVENT_KIND_PROXIMITY_OUT = 1, IDEATEK_PEN_EVENT_KIND_TIP_DOWN = 2, IDEATEK_PEN_EVENT_KIND_TIP_UP = 3, IDEATEK_PEN_EVENT_KIND_BUTTON_DOWN = 4, IDEATEK_PEN_EVENT_KIND_BUTTON_UP = 5, IDEATEK_PEN_EVENT_KIND_MOTION = 6, IDEATEK_PEN_EVENT_KIND_AXIS = 7 }
{ ideatek_pen_type_t type; }
{ ideatek_entity_t window; float x; float y; float axes[7]; ideatek_pen_input_t flags; }
{ ideatek_entity_t pen; ideatek_entity_t window; float x; float y; float value; ideatek_pen_input_t flags; uint32_t order; uint8_t button; ideatek_pen_axis_t axis; ideatek_pen_event_kind_t kind; bool eraser; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pen_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pen_axis_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pen_input_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pen_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Pen` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PenState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PenEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:Pen`, `ideatek:PenState` and `ideatek:PenEvent` | The field types |
| A reader | No type. | Its types and the field types |
