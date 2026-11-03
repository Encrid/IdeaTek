# include/input/ideatek_gamepad.h

`include/input/ideatek_gamepad.h` gives the types of the joysticks and of the gamepads.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The joysticks and the gamepads

A joystick is a device entity with `ideatek:Joystick` and `ideatek:JoystickState`.
It gives its axes, its buttons and its hats by number.
`ideatek:JoystickState` holds the first 16 axes, 128 buttons and 4 hats. The other controls give only events.

A gamepad is a joystick that a mapping describes. It also has `ideatek:Gamepad` and `ideatek:GamepadState`.
They give the controls by their place on the gamepad.

A gamepad gives only `ideatek:GamepadEvent`. Its `ideatek:JoystickState` changes, but gives no event.
A joystick with no mapping gives only `ideatek:JoystickEvent`.
When a mapping comes for a joystick, the input module attaches `ideatek:Gamepad` and spawns an event of `IDEATEK_INPUT_EVENT_KIND_REMAPPED`.
`mappings` adds mappings: `docs/extern/input/ideatek_input.h.md`, *The settings of the input module*.

An axis of a joystick goes from -32768 to 32767, as the device gives it.
A stick of a gamepad goes from -1 to 1, and y goes to the bottom. A trigger goes from 0 to 1.
With `merge_motion`, the changes of 1 axis in a round join into 1 event with the last value.
The game attaches `ideatek:JoystickRequest` to a joystick entity to rumble it or to set its LEDs.

## ideatek:Joystick

`ideatek:Joystick` tells the controls of a joystick.
`ideatek_joystick_t` takes 10 bytes. Its alignment is 2.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `axes` | `uint16_t` | 0 | The number of axes. |
| `buttons` | `uint16_t` | 2 | The number of buttons. |
| `hats` | `uint16_t` | 4 | The number of hats. |
| `balls` | `uint16_t` | 6 | The number of balls. |
| `type` | `ideatek_joystick_type_t` | 8 | The kind of the joystick. |

Byte 9 holds no field.

### ideatek:ideatek_joystick_type_t

`ideatek_joystick_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The joystick |
| --- | --- | --- |
| `IDEATEK_JOYSTICK_TYPE_UNKNOWN` | 0 | Not known. |
| `IDEATEK_JOYSTICK_TYPE_GAMEPAD` | 1 | A gamepad. |
| `IDEATEK_JOYSTICK_TYPE_WHEEL` | 2 | A wheel. |
| `IDEATEK_JOYSTICK_TYPE_ARCADE_STICK` | 3 | A stick of an arcade. |
| `IDEATEK_JOYSTICK_TYPE_FLIGHT_STICK` | 4 | A stick of a plane. |
| `IDEATEK_JOYSTICK_TYPE_DANCE_PAD` | 5 | A pad for a dance. |
| `IDEATEK_JOYSTICK_TYPE_GUITAR` | 6 | A guitar. |
| `IDEATEK_JOYSTICK_TYPE_DRUM_KIT` | 7 | A kit of drums. |
| `IDEATEK_JOYSTICK_TYPE_ARCADE_PAD` | 8 | A pad of an arcade. |
| `IDEATEK_JOYSTICK_TYPE_THROTTLE` | 9 | A throttle. |

## ideatek:JoystickState

`ideatek:JoystickState` holds the values of the controls of a joystick.
`ideatek_joystick_state_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `buttons` | `uint64_t[2]` | 0 | 128 bits: the bit `b % 64` of `buttons[b / 64]` is 1 while the button `b` is down. |
| `axes` | `int16_t[16]` | 16 | The value of each axis. |
| `hats` | `ideatek_hat_t[4]` | 48 | The value of each hat. |

Bytes 52 to 55 hold no field.

### ideatek:ideatek_hat_t

`ideatek_hat_t` is a bitmask of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The direction |
| --- | --- | --- |
| `IDEATEK_HAT_UP` | 0x01 | Up. |
| `IDEATEK_HAT_RIGHT` | 0x02 | Right. |
| `IDEATEK_HAT_DOWN` | 0x04 | Down. |
| `IDEATEK_HAT_LEFT` | 0x08 | Left. |

A hat at 0 is at the center.

## ideatek:JoystickEvent

`ideatek:JoystickEvent` tells that a control of a joystick changed.
`ideatek_joystick_event_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `joystick` | `ideatek_entity_t` | 0 | The joystick entity. |
| `order` | `uint32_t` | 8 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `index` | `uint32_t` | 12 | The number of the axis, of the button, of the hat or of the ball. |
| `value` | `int16_t` | 16 | With an axis: the new value. |
| `rel_x` | `int16_t` | 18 | With a ball: the motion on x. |
| `rel_y` | `int16_t` | 20 | With a ball: the motion on y. |
| `hat` | `ideatek_hat_t` | 22 | With a hat: the new value. |
| `kind` | `ideatek_joystick_event_kind_t` | 23 | The control that changed. |
| `down` | `bool` | 24 | With a button: `true` when it goes down. |

Bytes 25 to 31 hold no field.

### ideatek:ideatek_joystick_event_kind_t

`ideatek_joystick_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The control |
| --- | --- | --- |
| `IDEATEK_JOYSTICK_EVENT_KIND_AXIS` | 0 | An axis moved. |
| `IDEATEK_JOYSTICK_EVENT_KIND_BUTTON` | 1 | A button went down or up. |
| `IDEATEK_JOYSTICK_EVENT_KIND_HAT` | 2 | A hat moved. |
| `IDEATEK_JOYSTICK_EVENT_KIND_BALL` | 3 | A ball moved. |

## ideatek:Gamepad

`ideatek:Gamepad` tells the model of a gamepad.
`ideatek_gamepad_t` takes 5 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `type` | `ideatek_gamepad_type_t` | 0 | The model. |
| `labels` | `ideatek_face_label_t[4]` | 1 | The labels of the face buttons: south, east, west and north. |

### ideatek:ideatek_gamepad_type_t

`ideatek_gamepad_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The model |
| --- | --- | --- |
| `IDEATEK_GAMEPAD_TYPE_UNKNOWN` | 0 | Not known. |
| `IDEATEK_GAMEPAD_TYPE_STANDARD` | 1 | A gamepad of no known model. |
| `IDEATEK_GAMEPAD_TYPE_XBOX360` | 2 | An Xbox 360 controller. |
| `IDEATEK_GAMEPAD_TYPE_XBOXONE` | 3 | An Xbox One controller. |
| `IDEATEK_GAMEPAD_TYPE_PS3` | 4 | A PlayStation 3 controller. |
| `IDEATEK_GAMEPAD_TYPE_PS4` | 5 | A PlayStation 4 controller. |
| `IDEATEK_GAMEPAD_TYPE_PS5` | 6 | A PlayStation 5 controller. |
| `IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO` | 7 | A Nintendo Switch Pro controller. |
| `IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT` | 8 | A left Joy-Con. |
| `IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT` | 9 | A right Joy-Con. |
| `IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR` | 10 | A pair of Joy-Cons. |
| `IDEATEK_GAMEPAD_TYPE_GAMECUBE` | 11 | A GameCube controller. |
| `IDEATEK_GAMEPAD_TYPE_STEAM` | 12 | A Steam controller. |

### ideatek:ideatek_face_label_t

`ideatek_face_label_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The label |
| --- | --- | --- |
| `IDEATEK_FACE_LABEL_UNKNOWN` | 0 | No label. |
| `IDEATEK_FACE_LABEL_A` | 1 | A. |
| `IDEATEK_FACE_LABEL_B` | 2 | B. |
| `IDEATEK_FACE_LABEL_X` | 3 | X. |
| `IDEATEK_FACE_LABEL_Y` | 4 | Y. |
| `IDEATEK_FACE_LABEL_CROSS` | 5 | A cross. |
| `IDEATEK_FACE_LABEL_CIRCLE` | 6 | A circle. |
| `IDEATEK_FACE_LABEL_SQUARE` | 7 | A square. |
| `IDEATEK_FACE_LABEL_TRIANGLE` | 8 | A triangle. |

## ideatek:GamepadState

`ideatek:GamepadState` holds the values of the controls of a gamepad.
`ideatek_gamepad_state_t` takes 56 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `left_x` | `float` | 0 | The left stick on x, from the left to the right. |
| `left_y` | `float` | 4 | The left stick on y, from the top to the bottom. |
| `right_x` | `float` | 8 | The right stick on x. |
| `right_y` | `float` | 12 | The right stick on y. |
| `left_trigger` | `float` | 16 | The left trigger. |
| `right_trigger` | `float` | 20 | The right trigger. |
| `buttons` | `uint32_t` | 24 | A mask: the bit of each value of `ideatek_gamepad_button_t` that is down. |
| `pressed` | `bool[26]` | 28 | `pressed[b]` is `true` while the button `b` is down. |
| `capsense` | `uint8_t` | 54 | A mask: the bit of each value of `ideatek_capsense_t` that the user touches. |

Byte 55 holds no field.

### ideatek:ideatek_gamepad_button_t

`ideatek_gamepad_button_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The button |
| --- | --- | --- |
| `IDEATEK_GAMEPAD_BUTTON_SOUTH` | 0 | The bottom face button. |
| `IDEATEK_GAMEPAD_BUTTON_EAST` | 1 | The right face button. |
| `IDEATEK_GAMEPAD_BUTTON_WEST` | 2 | The left face button. |
| `IDEATEK_GAMEPAD_BUTTON_NORTH` | 3 | The top face button. |
| `IDEATEK_GAMEPAD_BUTTON_BACK` | 4 | The back button. |
| `IDEATEK_GAMEPAD_BUTTON_GUIDE` | 5 | The guide button. |
| `IDEATEK_GAMEPAD_BUTTON_START` | 6 | The start button. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_STICK` | 7 | A push of the left stick. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK` | 8 | A push of the right stick. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER` | 9 | The left shoulder. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER` | 10 | The right shoulder. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_UP` | 11 | Up on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN` | 12 | Down on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT` | 13 | Left on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT` | 14 | Right on the pad of directions. |
| `IDEATEK_GAMEPAD_BUTTON_MISC1` | 15 | An extra button, for example share. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE1` | 16 | The first right paddle. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE1` | 17 | The first left paddle. |
| `IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE2` | 18 | The second right paddle. |
| `IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE2` | 19 | The second left paddle. |
| `IDEATEK_GAMEPAD_BUTTON_TOUCHPAD` | 20 | A push of the touchpad. |
| `IDEATEK_GAMEPAD_BUTTON_MISC2` | 21 | An extra button. |
| `IDEATEK_GAMEPAD_BUTTON_MISC3` | 22 | An extra button. |
| `IDEATEK_GAMEPAD_BUTTON_MISC4` | 23 | An extra button. |
| `IDEATEK_GAMEPAD_BUTTON_MISC5` | 24 | An extra button. |
| `IDEATEK_GAMEPAD_BUTTON_MISC6` | 25 | An extra button. |

### ideatek:ideatek_gamepad_axis_t

`ideatek_gamepad_axis_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The axis |
| --- | --- | --- |
| `IDEATEK_GAMEPAD_AXIS_LEFTX` | 0 | The left stick on x. |
| `IDEATEK_GAMEPAD_AXIS_LEFTY` | 1 | The left stick on y. |
| `IDEATEK_GAMEPAD_AXIS_RIGHTX` | 2 | The right stick on x. |
| `IDEATEK_GAMEPAD_AXIS_RIGHTY` | 3 | The right stick on y. |
| `IDEATEK_GAMEPAD_AXIS_LEFT_TRIGGER` | 4 | The left trigger. |
| `IDEATEK_GAMEPAD_AXIS_RIGHT_TRIGGER` | 5 | The right trigger. |

### ideatek:ideatek_capsense_t

`ideatek_capsense_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The touch |
| --- | --- | --- |
| `IDEATEK_CAPSENSE_LEFT_STICK` | 0 | A touch on the top of the left stick. |
| `IDEATEK_CAPSENSE_RIGHT_STICK` | 1 | A touch on the top of the right stick. |
| `IDEATEK_CAPSENSE_LEFT_GRIP` | 2 | A hand on the left grip. |
| `IDEATEK_CAPSENSE_RIGHT_GRIP` | 3 | A hand on the right grip. |

## ideatek:GamepadEvent

`ideatek:GamepadEvent` tells that a control of a gamepad changed.
`ideatek_gamepad_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gamepad` | `ideatek_entity_t` | 0 | The gamepad entity. |
| `order` | `uint32_t` | 8 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `value` | `float` | 12 | With an axis: the new value. |
| `index` | `uint8_t` | 16 | The value of `ideatek_gamepad_axis_t`, `ideatek_gamepad_button_t` or `ideatek_capsense_t`. |
| `kind` | `ideatek_gamepad_event_kind_t` | 17 | The control that changed. |
| `down` | `bool` | 18 | With a button or a touch: `true` when it starts. |

Bytes 19 to 23 hold no field.

### ideatek:ideatek_gamepad_event_kind_t

`ideatek_gamepad_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The control |
| --- | --- | --- |
| `IDEATEK_GAMEPAD_EVENT_KIND_AXIS` | 0 | An axis. |
| `IDEATEK_GAMEPAD_EVENT_KIND_BUTTON` | 1 | A button. |
| `IDEATEK_GAMEPAD_EVENT_KIND_CAPSENSE` | 2 | A sensor of touch. |

## ideatek:JoystickRequest

`ideatek:JoystickRequest` asks a joystick to rumble or to set its LEDs.
`ideatek_joystick_request_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `duration_ms` | `uint32_t` | 0 | The length of a rumble, in milliseconds. |
| `trigger_duration_ms` | `uint32_t` | 4 | The length of a rumble of the triggers, in milliseconds. |
| `rumbles` | `uint32_t` | 8 | A count: each addition of 1 starts a rumble of `low` and `high`. |
| `trigger_rumbles` | `uint32_t` | 12 | A count: each addition of 1 starts a rumble of the triggers. |
| `player_index` | `int32_t` | 16 | The player index that the LEDs show, or -1. |
| `low` | `float` | 20 | The force of the motor of low frequency, from 0 to 1. |
| `high` | `float` | 24 | The force of the motor of high frequency, from 0 to 1. |
| `trigger_left` | `float` | 28 | The force of the left trigger, from 0 to 1. |
| `trigger_right` | `float` | 32 | The force of the right trigger, from 0 to 1. |
| `red` | `uint8_t` | 36 | The red of the LED. |
| `green` | `uint8_t` | 37 | The green of the LED. |
| `blue` | `uint8_t` | 38 | The blue of the LED. |

Byte 39 holds no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_joystick_type_t` | `ideatek_joystick_type_t` | `ideatek_joystick_type_t_desc` | `IDEATEK_JOYSTICK_TYPE_VALUES` |
| `ideatek:ideatek_hat_t` | `ideatek_hat_t` | `ideatek_hat_t_desc` | `IDEATEK_HAT_VALUES` |
| `ideatek:ideatek_joystick_event_kind_t` | `ideatek_joystick_event_kind_t` | `ideatek_joystick_event_kind_t_desc` | `IDEATEK_JOYSTICK_EVENT_KIND_VALUES` |
| `ideatek:ideatek_gamepad_type_t` | `ideatek_gamepad_type_t` | `ideatek_gamepad_type_t_desc` | `IDEATEK_GAMEPAD_TYPE_VALUES` |
| `ideatek:ideatek_face_label_t` | `ideatek_face_label_t` | `ideatek_face_label_t_desc` | `IDEATEK_FACE_LABEL_VALUES` |
| `ideatek:ideatek_gamepad_button_t` | `ideatek_gamepad_button_t` | `ideatek_gamepad_button_t_desc` | `IDEATEK_GAMEPAD_BUTTON_VALUES` |
| `ideatek:ideatek_gamepad_axis_t` | `ideatek_gamepad_axis_t` | `ideatek_gamepad_axis_t_desc` | `IDEATEK_GAMEPAD_AXIS_VALUES` |
| `ideatek:ideatek_capsense_t` | `ideatek_capsense_t` | `ideatek_capsense_t_desc` | `IDEATEK_CAPSENSE_VALUES` |
| `ideatek:ideatek_gamepad_event_kind_t` | `ideatek_gamepad_event_kind_t` | `ideatek_gamepad_event_kind_t_desc` | `IDEATEK_GAMEPAD_EVENT_KIND_VALUES` |
| `ideatek:Joystick` | `ideatek_joystick_t` | `ideatek_joystick_t_desc` | `IDEATEK_JOYSTICK_FIELDS` |
| `ideatek:JoystickState` | `ideatek_joystick_state_t` | `ideatek_joystick_state_t_desc` | `IDEATEK_JOYSTICK_STATE_FIELDS` |
| `ideatek:JoystickEvent` | `ideatek_joystick_event_t` | `ideatek_joystick_event_t_desc` | `IDEATEK_JOYSTICK_EVENT_FIELDS` |
| `ideatek:Gamepad` | `ideatek_gamepad_t` | `ideatek_gamepad_t_desc` | `IDEATEK_GAMEPAD_FIELDS` |
| `ideatek:GamepadState` | `ideatek_gamepad_state_t` | `ideatek_gamepad_state_t_desc` | `IDEATEK_GAMEPAD_STATE_FIELDS` |
| `ideatek:GamepadEvent` | `ideatek_gamepad_event_t` | `ideatek_gamepad_event_t_desc` | `IDEATEK_GAMEPAD_EVENT_FIELDS` |
| `ideatek:JoystickRequest` | `ideatek_joystick_request_t` | `ideatek_joystick_request_t_desc` | `IDEATEK_JOYSTICK_REQUEST_FIELDS` |

```text
{ IDEATEK_JOYSTICK_TYPE_UNKNOWN = 0, IDEATEK_JOYSTICK_TYPE_GAMEPAD = 1, IDEATEK_JOYSTICK_TYPE_WHEEL = 2, IDEATEK_JOYSTICK_TYPE_ARCADE_STICK = 3, IDEATEK_JOYSTICK_TYPE_FLIGHT_STICK = 4, IDEATEK_JOYSTICK_TYPE_DANCE_PAD = 5, IDEATEK_JOYSTICK_TYPE_GUITAR = 6, IDEATEK_JOYSTICK_TYPE_DRUM_KIT = 7, IDEATEK_JOYSTICK_TYPE_ARCADE_PAD = 8, IDEATEK_JOYSTICK_TYPE_THROTTLE = 9 }
{ IDEATEK_HAT_UP = 0x01, IDEATEK_HAT_RIGHT = 0x02, IDEATEK_HAT_DOWN = 0x04, IDEATEK_HAT_LEFT = 0x08 }
{ IDEATEK_JOYSTICK_EVENT_KIND_AXIS = 0, IDEATEK_JOYSTICK_EVENT_KIND_BUTTON = 1, IDEATEK_JOYSTICK_EVENT_KIND_HAT = 2, IDEATEK_JOYSTICK_EVENT_KIND_BALL = 3 }
{ IDEATEK_GAMEPAD_TYPE_UNKNOWN = 0, IDEATEK_GAMEPAD_TYPE_STANDARD = 1, IDEATEK_GAMEPAD_TYPE_XBOX360 = 2, IDEATEK_GAMEPAD_TYPE_XBOXONE = 3, IDEATEK_GAMEPAD_TYPE_PS3 = 4, IDEATEK_GAMEPAD_TYPE_PS4 = 5, IDEATEK_GAMEPAD_TYPE_PS5 = 6, IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO = 7, IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT = 8, IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT = 9, IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR = 10, IDEATEK_GAMEPAD_TYPE_GAMECUBE = 11, IDEATEK_GAMEPAD_TYPE_STEAM = 12 }
{ IDEATEK_FACE_LABEL_UNKNOWN = 0, IDEATEK_FACE_LABEL_A = 1, IDEATEK_FACE_LABEL_B = 2, IDEATEK_FACE_LABEL_X = 3, IDEATEK_FACE_LABEL_Y = 4, IDEATEK_FACE_LABEL_CROSS = 5, IDEATEK_FACE_LABEL_CIRCLE = 6, IDEATEK_FACE_LABEL_SQUARE = 7, IDEATEK_FACE_LABEL_TRIANGLE = 8 }
{ IDEATEK_GAMEPAD_BUTTON_SOUTH = 0, IDEATEK_GAMEPAD_BUTTON_EAST = 1, IDEATEK_GAMEPAD_BUTTON_WEST = 2, IDEATEK_GAMEPAD_BUTTON_NORTH = 3, IDEATEK_GAMEPAD_BUTTON_BACK = 4, IDEATEK_GAMEPAD_BUTTON_GUIDE = 5, IDEATEK_GAMEPAD_BUTTON_START = 6, IDEATEK_GAMEPAD_BUTTON_LEFT_STICK = 7, IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK = 8, IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER = 9, IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER = 10, IDEATEK_GAMEPAD_BUTTON_DPAD_UP = 11, IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN = 12, IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT = 13, IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT = 14, IDEATEK_GAMEPAD_BUTTON_MISC1 = 15, IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE1 = 16, IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE1 = 17, IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE2 = 18, IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE2 = 19, IDEATEK_GAMEPAD_BUTTON_TOUCHPAD = 20, IDEATEK_GAMEPAD_BUTTON_MISC2 = 21, IDEATEK_GAMEPAD_BUTTON_MISC3 = 22, IDEATEK_GAMEPAD_BUTTON_MISC4 = 23, IDEATEK_GAMEPAD_BUTTON_MISC5 = 24, IDEATEK_GAMEPAD_BUTTON_MISC6 = 25 }
{ IDEATEK_GAMEPAD_AXIS_LEFTX = 0, IDEATEK_GAMEPAD_AXIS_LEFTY = 1, IDEATEK_GAMEPAD_AXIS_RIGHTX = 2, IDEATEK_GAMEPAD_AXIS_RIGHTY = 3, IDEATEK_GAMEPAD_AXIS_LEFT_TRIGGER = 4, IDEATEK_GAMEPAD_AXIS_RIGHT_TRIGGER = 5 }
{ IDEATEK_CAPSENSE_LEFT_STICK = 0, IDEATEK_CAPSENSE_RIGHT_STICK = 1, IDEATEK_CAPSENSE_LEFT_GRIP = 2, IDEATEK_CAPSENSE_RIGHT_GRIP = 3 }
{ IDEATEK_GAMEPAD_EVENT_KIND_AXIS = 0, IDEATEK_GAMEPAD_EVENT_KIND_BUTTON = 1, IDEATEK_GAMEPAD_EVENT_KIND_CAPSENSE = 2 }
{ uint16_t axes; uint16_t buttons; uint16_t hats; uint16_t balls; ideatek_joystick_type_t type; }
{ uint64_t buttons[2]; int16_t axes[16]; ideatek_hat_t hats[4]; }
{ ideatek_entity_t joystick; uint32_t order; uint32_t index; int16_t value; int16_t rel_x; int16_t rel_y; ideatek_hat_t hat; ideatek_joystick_event_kind_t kind; bool down; }
{ ideatek_gamepad_type_t type; ideatek_face_label_t labels[4]; }
{ float left_x; float left_y; float right_x; float right_y; float left_trigger; float right_trigger; uint32_t buttons; bool pressed[26]; uint8_t capsense; }
{ ideatek_entity_t gamepad; uint32_t order; float value; uint8_t index; ideatek_gamepad_event_kind_t kind; bool down; }
{ uint32_t duration_ms; uint32_t trigger_duration_ms; uint32_t rumbles; uint32_t trigger_rumbles; int32_t player_index; float low; float high; float trigger_left; float trigger_right; uint8_t red; uint8_t green; uint8_t blue; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joystick_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_hat_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joystick_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gamepad_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_face_label_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gamepad_button_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gamepad_axis_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_capsense_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gamepad_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Joystick` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:JoystickState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:JoystickEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Gamepad` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GamepadState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GamepadEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:JoystickRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:JoystickRequest` | The field types |
| The input module | `ideatek:Joystick`, `ideatek:JoystickState`, `ideatek:JoystickEvent`, `ideatek:Gamepad`, `ideatek:GamepadState` and `ideatek:GamepadEvent` | `ideatek:JoystickRequest` and the field types |
| A reader | No type. | Its types and the field types |
