# include/platform_abstraction/input/ideatek_gamepad.h

`include/platform_abstraction/input/ideatek_gamepad.h` gives the types of the gamepads.
`docs/extern/platform_abstraction/input/ideatek_input.h.md` tells the common rules.

## The gamepads

A gamepad is a joystick that a mapping describes.
`mappings` adds mappings: `docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The settings of the input module*.
Each gamepad that connects gets a new entity with `ideatek:Gamepad` and `ideatek:GamepadState`.
When the gamepad disconnects, the input module destroys its entity.

A stick of a gamepad goes from -1 to 1, and y goes to the bottom. A trigger goes from 0 to 1.
Each button that goes down or up gives an `ideatek:GamepadEvent`.

The game attaches `ideatek:GamepadRequest` to the gamepad entity for a rumble.

Comes back: a model type when a game shows button prompts per model; raw joysticks when a game supports flight sticks or wheels.

## ideatek:Gamepad

`ideatek:Gamepad` names a gamepad.
`ideatek_gamepad_t` takes 379 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `char[379]` | 0 | The name of the gamepad in UTF-8, with a null at the end. |

A string descriptor of USB holds at most 126 code units of UTF-16. Each unit gives at most 3 bytes of UTF-8, so a name has at most 378 bytes and a null. A Bluetooth device name has at most 248 bytes of UTF-8. The input module cuts a longer name at the last whole code point.

## ideatek:GamepadState

`ideatek:GamepadState` keeps the last values of the sticks, the triggers and the buttons.
`ideatek_gamepad_state_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `left_x` | `float` | 0 | The left stick on x, from the left to the right. |
| `left_y` | `float` | 4 | The left stick on y, from the top to the bottom. |
| `right_x` | `float` | 8 | The right stick on x. |
| `right_y` | `float` | 12 | The right stick on y. |
| `left_trigger` | `float` | 16 | The left trigger. |
| `right_trigger` | `float` | 20 | The right trigger. |
| `buttons` | `ideatek_gamepad_button_t` | 24 | The buttons that are down. |

### ideatek:ideatek_gamepad_button_t

`ideatek_gamepad_button_t` is a bitmask of `uint32_t`. Its size and its alignment are 4.

The bit i is the button i of the standard mapping of the W3C Gamepad specification. The bits 6 and 7 have no constant, because `left_trigger` and `right_trigger` give the triggers.

| Constant | Value | The button |
| --- | --- | --- |
| `IDEATEK_GAMEPAD_BUTTON_SOUTH` | 0x00001 | The bottom face button. |
| `IDEATEK_GAMEPAD_BUTTON_EAST` | 0x00002 | The right face button. |
| `IDEATEK_GAMEPAD_BUTTON_WEST` | 0x00004 | The left face button. |
| `IDEATEK_GAMEPAD_BUTTON_NORTH` | 0x00008 | The top face button. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER` | 0x00010 | The left shoulder. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER` | 0x00020 | The right shoulder. |
| `IDEATEK_GAMEPAD_BUTTON_BACK` | 0x00100 | The back button. |
| `IDEATEK_GAMEPAD_BUTTON_START` | 0x00200 | The start button. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_STICK` | 0x00400 | A push of the left stick. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK` | 0x00800 | A push of the right stick. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_UP` | 0x01000 | Up on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN` | 0x02000 | Down on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT` | 0x04000 | Left on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT` | 0x08000 | Right on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_GUIDE` | 0x10000 | The guide button. |

## ideatek:GamepadEvent

`ideatek:GamepadEvent` tells that a button of a gamepad goes down or up.
`ideatek_gamepad_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gamepad` | `ideatek_entity_t` | 0 | The gamepad entity. |
| `order` | `uint32_t` | 8 | The rank of the event: `docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The events*. |
| `button` | `ideatek_gamepad_button_t` | 12 | The button, with 1 bit. |
| `down` | `bool` | 16 | `true` when the button goes down. |

Bytes 17 to 23 hold no field.

## ideatek:GamepadRequest

`ideatek:GamepadRequest` asks for a rumble.
`ideatek_gamepad_request_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `weak` | `float` | 0 | The force of the motor of high frequency, from 0 to 1. |
| `strong` | `float` | 4 | The force of the motor of low frequency, from 0 to 1. |
| `duration_ms` | `uint32_t` | 8 | The length of a rumble, in milliseconds. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_gamepad_button_t` | `ideatek_gamepad_button_t` | `ideatek_gamepad_button_t_desc` | `IDEATEK_GAMEPAD_BUTTON_VALUES` |
| `ideatek:Gamepad` | `ideatek_gamepad_t` | `ideatek_gamepad_t_desc` | `IDEATEK_GAMEPAD_FIELDS` |
| `ideatek:GamepadState` | `ideatek_gamepad_state_t` | `ideatek_gamepad_state_t_desc` | `IDEATEK_GAMEPAD_STATE_FIELDS` |
| `ideatek:GamepadEvent` | `ideatek_gamepad_event_t` | `ideatek_gamepad_event_t_desc` | `IDEATEK_GAMEPAD_EVENT_FIELDS` |
| `ideatek:GamepadRequest` | `ideatek_gamepad_request_t` | `ideatek_gamepad_request_t_desc` | `IDEATEK_GAMEPAD_REQUEST_FIELDS` |

```text
{ IDEATEK_GAMEPAD_BUTTON_SOUTH = 0x00001, IDEATEK_GAMEPAD_BUTTON_EAST = 0x00002, IDEATEK_GAMEPAD_BUTTON_WEST = 0x00004, IDEATEK_GAMEPAD_BUTTON_NORTH = 0x00008, IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER = 0x00010, IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER = 0x00020, IDEATEK_GAMEPAD_BUTTON_BACK = 0x00100, IDEATEK_GAMEPAD_BUTTON_START = 0x00200, IDEATEK_GAMEPAD_BUTTON_LEFT_STICK = 0x00400, IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK = 0x00800, IDEATEK_GAMEPAD_BUTTON_DPAD_UP = 0x01000, IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN = 0x02000, IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT = 0x04000, IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT = 0x08000, IDEATEK_GAMEPAD_BUTTON_GUIDE = 0x10000 }
{ char name[379]; }
{ float left_x; float left_y; float right_x; float right_y; float left_trigger; float right_trigger; ideatek_gamepad_button_t buttons; }
{ ideatek_entity_t gamepad; uint32_t order; ideatek_gamepad_button_t button; bool down; }
{ float weak; float strong; uint32_t duration_ms; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gamepad_button_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Gamepad` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GamepadState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GamepadEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GamepadRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:GamepadRequest` | The field types |
| The input module | `ideatek:Gamepad`, `ideatek:GamepadState` and `ideatek:GamepadEvent` | `ideatek:GamepadRequest` and the field types |
| A reader | No type. | Its types and the field types |
