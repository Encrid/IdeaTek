# include/input/ideatek_mouse.h

`include/input/ideatek_mouse.h` gives the types of the mouse and of the cursor.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The mouse

The button 1 is the left button, 2 the middle button and 3 the right button. 4 and 5 are side buttons.
A mask of buttons has the bit `button - 1` for each button that is down.
With `merge_motion`, `rel_x` and `rel_y` of a motion add up the motions that it joins.
A wheel gives ticks. A tick is 1 step of a wheel, and a fraction of a tick comes from a smooth wheel or a touchpad.

The game attaches `ideatek:MouseRequest` to the input entity, the entity of `ideatek:Mouse`.

## ideatek:Mouse

`ideatek:Mouse` keeps the last place and the buttons of the mouse.
`ideatek_mouse_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The place of the mouse on x in the window, in pixels. |
| `y` | `float` | 4 | The place of the mouse on y in the window, in pixels. |
| `buttons` | `uint32_t` | 8 | The mask of the buttons that are down. |

## ideatek:MouseMotion

`ideatek:MouseMotion` tells that the mouse moves.
`ideatek_mouse_motion_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The place of the mouse on x in the window, in pixels. |
| `y` | `float` | 4 | The place of the mouse on y in the window, in pixels. |
| `rel_x` | `float` | 8 | The motion on x since the last event. |
| `rel_y` | `float` | 12 | The motion on y since the last event. |
| `buttons` | `uint32_t` | 16 | The mask of the buttons that are down. |
| `order` | `uint32_t` | 20 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |

## ideatek:MouseButton

`ideatek:MouseButton` tells that a button goes down or up.
`ideatek_mouse_button_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The place of the mouse on x in the window, in pixels. |
| `y` | `float` | 4 | The place of the mouse on y in the window, in pixels. |
| `order` | `uint32_t` | 8 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `button` | `uint8_t` | 12 | The button: *The mouse*. |
| `down` | `bool` | 13 | `true` when the button goes down. |

Bytes 14 and 15 hold no field.

## ideatek:MouseWheel

`ideatek:MouseWheel` tells that a wheel turns.
`ideatek_mouse_wheel_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `ticks_x` | `float` | 0 | The ticks on x. A positive value goes to the right. |
| `ticks_y` | `float` | 4 | The ticks on y. A positive value goes away from the user. |
| `order` | `uint32_t` | 8 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |

## ideatek:MouseRequest

`ideatek:MouseRequest` asks for a mode of the mouse and for a cursor.
`ideatek_mouse_request_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `mode` | `ideatek_mouse_mode_t` | 0 | The mode of the mouse. |
| `cursor` | `ideatek_cursor_t` | 1 | The cursor that shows in the mode `IDEATEK_MOUSE_MODE_NORMAL`. |

### ideatek:ideatek_mouse_mode_t

`ideatek_mouse_mode_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The mode |
| --- | --- | --- |
| `IDEATEK_MOUSE_MODE_NORMAL` | 0 | The mouse moves freely, and the cursor shows. |
| `IDEATEK_MOUSE_MODE_HIDDEN` | 1 | The cursor hides. |
| `IDEATEK_MOUSE_MODE_RELATIVE` | 2 | The cursor hides, and the mouse gives only motions. `x` and `y` stay. |

### ideatek:ideatek_cursor_t

`ideatek_cursor_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The cursor |
| --- | --- | --- |
| `IDEATEK_CURSOR_DEFAULT` | 0 | The cursor of the system. |
| `IDEATEK_CURSOR_ARROW` | 1 | The arrow. |
| `IDEATEK_CURSOR_IBEAM` | 2 | The cursor of a text. |
| `IDEATEK_CURSOR_CROSSHAIR` | 3 | A cross. |
| `IDEATEK_CURSOR_POINTING_HAND` | 4 | A hand that points. |
| `IDEATEK_CURSOR_RESIZE_EW` | 5 | Two arrows, to the left and the right. |
| `IDEATEK_CURSOR_RESIZE_NS` | 6 | Two arrows, to the top and the bottom. |
| `IDEATEK_CURSOR_RESIZE_NWSE` | 7 | Two arrows, to the top left and the bottom right. |
| `IDEATEK_CURSOR_RESIZE_NESW` | 8 | Two arrows, to the top right and the bottom left. |
| `IDEATEK_CURSOR_RESIZE_ALL` | 9 | Four arrows. |
| `IDEATEK_CURSOR_NOT_ALLOWED` | 10 | A circle with a line through it. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_mouse_mode_t` | `ideatek_mouse_mode_t` | `ideatek_mouse_mode_t_desc` | `IDEATEK_MOUSE_MODE_VALUES` |
| `ideatek:ideatek_cursor_t` | `ideatek_cursor_t` | `ideatek_cursor_t_desc` | `IDEATEK_CURSOR_VALUES` |
| `ideatek:Mouse` | `ideatek_mouse_t` | `ideatek_mouse_t_desc` | `IDEATEK_MOUSE_FIELDS` |
| `ideatek:MouseMotion` | `ideatek_mouse_motion_t` | `ideatek_mouse_motion_t_desc` | `IDEATEK_MOUSE_MOTION_FIELDS` |
| `ideatek:MouseButton` | `ideatek_mouse_button_t` | `ideatek_mouse_button_t_desc` | `IDEATEK_MOUSE_BUTTON_FIELDS` |
| `ideatek:MouseWheel` | `ideatek_mouse_wheel_t` | `ideatek_mouse_wheel_t_desc` | `IDEATEK_MOUSE_WHEEL_FIELDS` |
| `ideatek:MouseRequest` | `ideatek_mouse_request_t` | `ideatek_mouse_request_t_desc` | `IDEATEK_MOUSE_REQUEST_FIELDS` |

```text
{ IDEATEK_MOUSE_MODE_NORMAL = 0, IDEATEK_MOUSE_MODE_HIDDEN = 1, IDEATEK_MOUSE_MODE_RELATIVE = 2 }
{ IDEATEK_CURSOR_DEFAULT = 0, IDEATEK_CURSOR_ARROW = 1, IDEATEK_CURSOR_IBEAM = 2, IDEATEK_CURSOR_CROSSHAIR = 3, IDEATEK_CURSOR_POINTING_HAND = 4, IDEATEK_CURSOR_RESIZE_EW = 5, IDEATEK_CURSOR_RESIZE_NS = 6, IDEATEK_CURSOR_RESIZE_NWSE = 7, IDEATEK_CURSOR_RESIZE_NESW = 8, IDEATEK_CURSOR_RESIZE_ALL = 9, IDEATEK_CURSOR_NOT_ALLOWED = 10 }
{ float x; float y; uint32_t buttons; }
{ float x; float y; float rel_x; float rel_y; uint32_t buttons; uint32_t order; }
{ float x; float y; uint32_t order; uint8_t button; bool down; }
{ float ticks_x; float ticks_y; uint32_t order; }
{ ideatek_mouse_mode_t mode; ideatek_cursor_t cursor; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_mouse_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_cursor_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Mouse` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MouseMotion` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MouseButton` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MouseWheel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MouseRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:MouseRequest` | The field types |
| The input module | `ideatek:Mouse`, `ideatek:MouseMotion`, `ideatek:MouseButton` and `ideatek:MouseWheel` | `ideatek:MouseRequest` and the field types |
| A reader | No type. | Its types and the field types |
