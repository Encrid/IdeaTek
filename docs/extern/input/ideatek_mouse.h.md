# include/input/ideatek_mouse.h

`include/input/ideatek_mouse.h` gives the types of the mice and of the cursor.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The mice

A mouse is a device entity with `ideatek:Mouse`.
A touch and a pen can also give mouse events: `docs/extern/input/ideatek_input.h.md`, *The settings of the input module*.
`source` of an event tells the device that gave it.
The button 1 is the left button, 2 the middle button and 3 the right button. 4 and 5 are side buttons.
A mask of buttons has the bit `button - 1` for each button that is down.

## The cursor

The game attaches `ideatek:MouseRequest` to a window entity.
With `custom` at `true`, the cursor is the image of `pixels`. Else it is `cursor`.
If the system has no such cursor, `cursor_found` of `ideatek:Mouse` is `false`.
If the system cannot move the mouse for a warp, `warp_works` of `ideatek:Mouse` is `false`.
With `merge_motion`, `rel_x` and `rel_y` of a motion add up the motions that it joins.

## ideatek:Mouse

`ideatek:Mouse` holds the state of a mouse.
`ideatek_mouse_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window under the mouse, or 0. |
| `x` | `float` | 8 | The place of the mouse on x in the window, in pixels. |
| `y` | `float` | 12 | The place of the mouse on y in the window, in pixels. |
| `buttons` | `uint32_t` | 16 | The mask of the buttons that are down. |
| `relative` | `bool` | 20 | `true` while the mouse is in the relative mode. |
| `cursor_found` | `bool` | 21 | `false` if the system has no cursor that the request asks for. |
| `warp_works` | `bool` | 22 | `false` if the last warp failed. |

Byte 23 holds no field.

## ideatek:MouseMotion

`ideatek:MouseMotion` tells that a mouse moved.
`ideatek_mouse_motion_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `mouse` | `ideatek_entity_t` | 0 | The mouse entity. |
| `window` | `ideatek_entity_t` | 8 | The window under the mouse, or 0. |
| `x` | `float` | 16 | The new place on x, in pixels. |
| `y` | `float` | 20 | The new place on y, in pixels. |
| `rel_x` | `float` | 24 | The motion on x since the last event. |
| `rel_y` | `float` | 28 | The motion on y since the last event. |
| `buttons` | `uint32_t` | 32 | The mask of the buttons that are down. |
| `order` | `uint32_t` | 36 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `source` | `ideatek_source_t` | 40 | The device that gave the event. |

Bytes 41 to 47 hold no field.

### ideatek:ideatek_source_t

`ideatek_source_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The device |
| --- | --- | --- |
| `IDEATEK_SOURCE_MOUSE` | 0 | A mouse. |
| `IDEATEK_SOURCE_TOUCH` | 1 | A touch device. |
| `IDEATEK_SOURCE_PEN` | 2 | A pen. |

## ideatek:MouseButton

`ideatek:MouseButton` tells that a button went down or up.
`ideatek_mouse_button_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `mouse` | `ideatek_entity_t` | 0 | The mouse entity. |
| `window` | `ideatek_entity_t` | 8 | The window under the mouse, or 0. |
| `x` | `float` | 16 | The place on x, in pixels. |
| `y` | `float` | 20 | The place on y, in pixels. |
| `order` | `uint32_t` | 24 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `button` | `uint8_t` | 28 | The button: *The mice*. |
| `clicks` | `uint8_t` | 29 | 1 for a single click, 2 for a double click, and so on. |
| `source` | `ideatek_source_t` | 30 | The device that gave the event. |
| `down` | `bool` | 31 | `true` when the button goes down. |

## ideatek:MouseWheel

`ideatek:MouseWheel` tells that a wheel turned.
`ideatek_mouse_wheel_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `mouse` | `ideatek_entity_t` | 0 | The mouse entity. |
| `window` | `ideatek_entity_t` | 8 | The window under the mouse, or 0. |
| `x` | `float` | 16 | The amount on x, above 0 to the right. |
| `y` | `float` | 20 | The amount on y, above 0 away from the user. |
| `ticks_x` | `int32_t` | 24 | The whole ticks on x. |
| `ticks_y` | `int32_t` | 28 | The whole ticks on y. |
| `mouse_x` | `float` | 32 | The place of the mouse on x, in pixels. |
| `mouse_y` | `float` | 36 | The place of the mouse on y, in pixels. |
| `order` | `uint32_t` | 40 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `flipped` | `bool` | 44 | `true` if the system flips the direction. |

Bytes 45 to 47 hold no field.

## ideatek:MouseRequest

`ideatek:MouseRequest` asks for a cursor and a mode of the mouse in a window.
`ideatek_mouse_request_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window of the request. |
| `pixels` | `bridge_text_t` | 8 | With `custom`: the image, 4 bytes for each pixel, red, green, blue and alpha. |
| `width` | `uint32_t` | 16 | The width of the image, in pixels. |
| `height` | `uint32_t` | 20 | The height of the image, in pixels. |
| `hot_x` | `int32_t` | 24 | The point on x of the image that is the tip of the cursor. |
| `hot_y` | `int32_t` | 28 | The point on y of the image that is the tip of the cursor. |
| `warp_x` | `float` | 32 | The place on x of a warp, in the window. |
| `warp_y` | `float` | 36 | The place on y of a warp, in the window. |
| `warps` | `uint32_t` | 40 | A count: each addition of 1 moves the mouse to `warp_x` and `warp_y`. |
| `mode` | `ideatek_mouse_mode_t` | 44 | How the mouse moves. |
| `cursor` | `ideatek_cursor_t` | 45 | Without `custom`: the cursor of the system. |
| `capture` | `bool` | 46 | `true` keeps the events while a button is down and the mouse is out of the window. |
| `custom` | `bool` | 47 | `true` uses `pixels` as the cursor. |

### ideatek:ideatek_mouse_mode_t

`ideatek_mouse_mode_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The mode |
| --- | --- | --- |
| `IDEATEK_MOUSE_MODE_NORMAL` | 0 | The mouse moves freely, and the cursor shows. |
| `IDEATEK_MOUSE_MODE_HIDDEN` | 1 | The cursor hides. |
| `IDEATEK_MOUSE_MODE_CONFINED` | 2 | The mouse stays in the window. |
| `IDEATEK_MOUSE_MODE_RELATIVE` | 3 | The cursor hides, and the mouse gives only motions. `x` and `y` stay. |

### ideatek:ideatek_cursor_t

`ideatek_cursor_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The cursor |
| --- | --- | --- |
| `IDEATEK_CURSOR_DEFAULT` | 0 | The arrow. |
| `IDEATEK_CURSOR_TEXT` | 1 | The cursor of a text. |
| `IDEATEK_CURSOR_WAIT` | 2 | The wait cursor. |
| `IDEATEK_CURSOR_CROSSHAIR` | 3 | A cross. |
| `IDEATEK_CURSOR_PROGRESS` | 4 | The arrow with a wait cursor. |
| `IDEATEK_CURSOR_NWSE_RESIZE` | 5 | Two arrows, to the top left and the bottom right. |
| `IDEATEK_CURSOR_NESW_RESIZE` | 6 | Two arrows, to the top right and the bottom left. |
| `IDEATEK_CURSOR_EW_RESIZE` | 7 | Two arrows, to the left and the right. |
| `IDEATEK_CURSOR_NS_RESIZE` | 8 | Two arrows, to the top and the bottom. |
| `IDEATEK_CURSOR_MOVE` | 9 | Four arrows. |
| `IDEATEK_CURSOR_NOT_ALLOWED` | 10 | A circle with a line through it. |
| `IDEATEK_CURSOR_POINTER` | 11 | A hand that points. |
| `IDEATEK_CURSOR_NW_RESIZE` | 12 | An arrow to the top left. |
| `IDEATEK_CURSOR_N_RESIZE` | 13 | An arrow to the top. |
| `IDEATEK_CURSOR_NE_RESIZE` | 14 | An arrow to the top right. |
| `IDEATEK_CURSOR_E_RESIZE` | 15 | An arrow to the right. |
| `IDEATEK_CURSOR_SE_RESIZE` | 16 | An arrow to the bottom right. |
| `IDEATEK_CURSOR_S_RESIZE` | 17 | An arrow to the bottom. |
| `IDEATEK_CURSOR_SW_RESIZE` | 18 | An arrow to the bottom left. |
| `IDEATEK_CURSOR_W_RESIZE` | 19 | An arrow to the left. |
| `IDEATEK_CURSOR_CONTEXT_MENU` | 20 | The arrow with a menu. |
| `IDEATEK_CURSOR_HELP` | 21 | The arrow with a question mark. |
| `IDEATEK_CURSOR_CELL` | 22 | A thick cross, for a cell. |
| `IDEATEK_CURSOR_VERTICAL_TEXT` | 23 | The cursor of a vertical text. |
| `IDEATEK_CURSOR_ALIAS` | 24 | The arrow with a link. |
| `IDEATEK_CURSOR_COPY` | 25 | The arrow with a plus. |
| `IDEATEK_CURSOR_NO_DROP` | 26 | The arrow with a circle and a line. |
| `IDEATEK_CURSOR_GRAB` | 27 | An open hand. |
| `IDEATEK_CURSOR_GRABBING` | 28 | A closed hand. |
| `IDEATEK_CURSOR_COL_RESIZE` | 29 | Two arrows to the left and the right, with a line between them. |
| `IDEATEK_CURSOR_ROW_RESIZE` | 30 | Two arrows to the top and the bottom, with a line between them. |
| `IDEATEK_CURSOR_ALL_SCROLL` | 31 | Arrows in each direction, with a point. |
| `IDEATEK_CURSOR_ZOOM_IN` | 32 | A magnifier with a plus. |
| `IDEATEK_CURSOR_ZOOM_OUT` | 33 | A magnifier with a minus. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_source_t` | `ideatek_source_t` | `ideatek_source_t_desc` | `IDEATEK_SOURCE_VALUES` |
| `ideatek:ideatek_mouse_mode_t` | `ideatek_mouse_mode_t` | `ideatek_mouse_mode_t_desc` | `IDEATEK_MOUSE_MODE_VALUES` |
| `ideatek:ideatek_cursor_t` | `ideatek_cursor_t` | `ideatek_cursor_t_desc` | `IDEATEK_CURSOR_VALUES` |
| `ideatek:Mouse` | `ideatek_mouse_t` | `ideatek_mouse_t_desc` | `IDEATEK_MOUSE_FIELDS` |
| `ideatek:MouseMotion` | `ideatek_mouse_motion_t` | `ideatek_mouse_motion_t_desc` | `IDEATEK_MOUSE_MOTION_FIELDS` |
| `ideatek:MouseButton` | `ideatek_mouse_button_t` | `ideatek_mouse_button_t_desc` | `IDEATEK_MOUSE_BUTTON_FIELDS` |
| `ideatek:MouseWheel` | `ideatek_mouse_wheel_t` | `ideatek_mouse_wheel_t_desc` | `IDEATEK_MOUSE_WHEEL_FIELDS` |
| `ideatek:MouseRequest` | `ideatek_mouse_request_t` | `ideatek_mouse_request_t_desc` | `IDEATEK_MOUSE_REQUEST_FIELDS` |

```text
{ IDEATEK_SOURCE_MOUSE = 0, IDEATEK_SOURCE_TOUCH = 1, IDEATEK_SOURCE_PEN = 2 }
{ IDEATEK_MOUSE_MODE_NORMAL = 0, IDEATEK_MOUSE_MODE_HIDDEN = 1, IDEATEK_MOUSE_MODE_CONFINED = 2, IDEATEK_MOUSE_MODE_RELATIVE = 3 }
{ IDEATEK_CURSOR_DEFAULT = 0, IDEATEK_CURSOR_TEXT = 1, IDEATEK_CURSOR_WAIT = 2, IDEATEK_CURSOR_CROSSHAIR = 3, IDEATEK_CURSOR_PROGRESS = 4, IDEATEK_CURSOR_NWSE_RESIZE = 5, IDEATEK_CURSOR_NESW_RESIZE = 6, IDEATEK_CURSOR_EW_RESIZE = 7, IDEATEK_CURSOR_NS_RESIZE = 8, IDEATEK_CURSOR_MOVE = 9, IDEATEK_CURSOR_NOT_ALLOWED = 10, IDEATEK_CURSOR_POINTER = 11, IDEATEK_CURSOR_NW_RESIZE = 12, IDEATEK_CURSOR_N_RESIZE = 13, IDEATEK_CURSOR_NE_RESIZE = 14, IDEATEK_CURSOR_E_RESIZE = 15, IDEATEK_CURSOR_SE_RESIZE = 16, IDEATEK_CURSOR_S_RESIZE = 17, IDEATEK_CURSOR_SW_RESIZE = 18, IDEATEK_CURSOR_W_RESIZE = 19, IDEATEK_CURSOR_CONTEXT_MENU = 20, IDEATEK_CURSOR_HELP = 21, IDEATEK_CURSOR_CELL = 22, IDEATEK_CURSOR_VERTICAL_TEXT = 23, IDEATEK_CURSOR_ALIAS = 24, IDEATEK_CURSOR_COPY = 25, IDEATEK_CURSOR_NO_DROP = 26, IDEATEK_CURSOR_GRAB = 27, IDEATEK_CURSOR_GRABBING = 28, IDEATEK_CURSOR_COL_RESIZE = 29, IDEATEK_CURSOR_ROW_RESIZE = 30, IDEATEK_CURSOR_ALL_SCROLL = 31, IDEATEK_CURSOR_ZOOM_IN = 32, IDEATEK_CURSOR_ZOOM_OUT = 33 }
{ ideatek_entity_t window; float x; float y; uint32_t buttons; bool relative; bool cursor_found; bool warp_works; }
{ ideatek_entity_t mouse; ideatek_entity_t window; float x; float y; float rel_x; float rel_y; uint32_t buttons; uint32_t order; ideatek_source_t source; }
{ ideatek_entity_t mouse; ideatek_entity_t window; float x; float y; uint32_t order; uint8_t button; uint8_t clicks; ideatek_source_t source; bool down; }
{ ideatek_entity_t mouse; ideatek_entity_t window; float x; float y; int32_t ticks_x; int32_t ticks_y; float mouse_x; float mouse_y; uint32_t order; bool flipped; }
{ ideatek_entity_t window; bridge_text_t pixels; uint32_t width; uint32_t height; int32_t hot_x; int32_t hot_y; float warp_x; float warp_y; uint32_t warps; ideatek_mouse_mode_t mode; ideatek_cursor_t cursor; bool capture; bool custom; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_source_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
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
