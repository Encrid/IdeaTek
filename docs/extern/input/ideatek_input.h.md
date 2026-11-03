# include/input/ideatek_input.h

`include/input/ideatek_input.h` gives the devices of the part `input`, and the rules of each input header.
`readme.md`, *The contracts*, tells the rules of each contract.
Each header includes `include/world/ideatek_world.h` for `ideatek_entity_t`, which `docs/extern/world/ideatek_world.h.md` describes.

| Header | Family | Document |
| --- | --- | --- |
| `include/input/ideatek_input.h` | The devices and the events of a device | This document |
| `include/input/ideatek_keyboard.h` | The keyboards | `docs/extern/input/ideatek_keyboard.h.md` |
| `include/input/ideatek_text.h` | The text input | `docs/extern/input/ideatek_text.h.md` |
| `include/input/ideatek_mouse.h` | The mice and the cursor | `docs/extern/input/ideatek_mouse.h.md` |
| `include/input/ideatek_gamepad.h` | The joysticks and the gamepads | `docs/extern/input/ideatek_gamepad.h.md` |
| `include/input/ideatek_touch.h` | The touch devices and the pinches | `docs/extern/input/ideatek_touch.h.md` |
| `include/input/ideatek_pen.h` | The pens | `docs/extern/input/ideatek_pen.h.md` |
| `include/input/ideatek_sensor.h` | The sensors | `docs/extern/input/ideatek_sensor.h.md` |

## The devices

The input module spawns 1 entity for each device that connects.
It attaches `ideatek:InputDevice`, `ideatek:InputUser` and `ideatek:DeviceState` to it.
It also attaches the types of the family of the device, for example `ideatek:Keyboard` and `ideatek:KeyLayout` to a keyboard.
When the device disconnects, the input module destroys its entity.
In the first round, each device that is present gives an event of `IDEATEK_INPUT_EVENT_KIND_CONNECTED`.

A gamepad is 1 entity with `ideatek:Joystick` and `ideatek:Gamepad`.
Its touchpad and each of its sensors are other entities, which name the gamepad.
On a system that has no users, each device has the user 1.
The keys and the motions of no known keyboard or mouse go to 1 virtual keyboard and 1 virtual mouse.
The input module spawns these 2 entities at `init`.

## The events

Each event is an entity that the input module spawns in the round of the event.
The input module destroys each event entity in the next round.
A reader hears each event with an observer of `BRIDGE_ON_ENTER`.
`order` of an event is its rank among all the input events of the round, from 0.
Thus a reader can sort the keys and the texts in the order of the user.

A state type keeps the last values of a device.
The input module attaches a new state only when its bytes change.

## The focus

When a window loses the keyboard focus, the input module releases each key that is down.
For each key, it spawns an `ideatek:KeyEvent` with `down` at `false`.
It also releases each button of a mouse that is down, with an `ideatek:MouseButton`.
Each finger gives an `ideatek:FingerEvent` of `IDEATEK_FINGER_EVENT_KIND_CANCELED`.
Each pen that touches gives an `ideatek:PenEvent` of `IDEATEK_PEN_EVENT_KIND_TIP_UP`.
Then it spawns an `ideatek:InputEvent` with `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST`.

## The requests

The game attaches a request to an entity to ask for an action.
When the game attaches the request again, the input module applies each field that changed.
A count asks for 1 action each time that the game adds 1 to it.
The counts are `clears`, `warps`, `rumbles` and `trigger_rumbles`.

## The blocks

At `init`, the input module allocates `device_bytes` and `text_bytes` bytes of its persistent share.
The `persistent` of its manifest holds the two numbers.
`device_bytes` holds the names, the serials and the fingers of the devices.
The bytes of a device are free again when the device disconnects.

`text_bytes` holds the texts of `ideatek:TextEvent`.
Each round writes its texts in the half that the last round did not use.
Thus a text stays good while its event entity lives.
If a text or a device does not fit, the input module drops it and writes 1 line in the log.

## The roles

- The game writes `ideatek:TextRequest`, `ideatek:MouseRequest`, `ideatek:JoystickRequest` and `ideatek:SensorRequest`.
- The input module reads these 4 types, and writes each other struct of the part. A deployment has 1 input module.
- A reader reads the types that it needs. The game and the UI are readers.

## The settings of the input module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `merge_motion` | `true` | `true` or `false` | `true` joins the motions of a mouse, a finger or a pen, and the changes of an axis, that follow each other into 1 event. |
| `mappings` | `[]` | a list of texts | The lines of gamepad mappings, in the format of SDL 3, that the input module adds. |
| `device_bytes` | 65536 | 1 to 1073741824 | The bytes for the names, the serials and the fingers. |
| `text_bytes` | 65536 | 2 to 1073741824, an even number | The bytes for the texts of the events. |
| `touch_fingers` | 10 | 1 to 64 | The most fingers that `ideatek:TouchState` holds for 1 device. |
| `touch_mouse_events` | `true` | `true` or `false` | `true` also gives a touch as mouse events. |
| `pen_mouse_events` | `true` | `true` or `false` | `true` also gives a pen as mouse events. |
| `pen_touch_events` | `true` | `true` or `false` | `true` also gives a pen as touch events. |
| `mouse_touch_events` | `false` | `true` or `false` | `true` also gives a mouse as touch events. |
| `gamepad_background` | `false` | `true` or `false` | `true` keeps the events of the joysticks while no window has the focus. |

## ideatek:InputDevice

`ideatek:InputDevice` tells what a device is. The input module attaches it once, when the device connects.
`ideatek_input_device_t` takes 44 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name, in UTF-8, in `device_bytes`. |
| `serial` | `bridge_text_t` | 8 | The serial number, in UTF-8, or no bytes. |
| `vendor` | `uint16_t` | 16 | The USB number of the vendor, or 0. |
| `product` | `uint16_t` | 18 | The USB number of the product, or 0. |
| `version` | `uint16_t` | 20 | The version of the product, or 0. |
| `guid` | `uint8_t[16]` | 22 | The GUID of a joystick, as SDL 3 gives it. Other devices have zeroes. |
| `kind` | `ideatek_device_kind_t` | 38 | The family of the device. |
| `rumble` | `bool` | 39 | `true` if the device can rumble. |
| `trigger_rumble` | `bool` | 40 | `true` if the triggers can rumble. |
| `mono_led` | `bool` | 41 | `true` if the device has a LED of 1 color. |
| `rgb_led` | `bool` | 42 | `true` if the device has a LED of red, green and blue. |
| `player_led` | `bool` | 43 | `true` if the device has LEDs that show the player. |

### ideatek:ideatek_device_kind_t

`ideatek_device_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The family |
| --- | --- | --- |
| `IDEATEK_DEVICE_KIND_KEYBOARD` | 0 | A keyboard. |
| `IDEATEK_DEVICE_KIND_MOUSE` | 1 | A mouse. |
| `IDEATEK_DEVICE_KIND_JOYSTICK` | 2 | A joystick or a gamepad. |
| `IDEATEK_DEVICE_KIND_TOUCH` | 3 | A touch device. |
| `IDEATEK_DEVICE_KIND_PEN` | 4 | A pen. |
| `IDEATEK_DEVICE_KIND_SENSOR` | 5 | A sensor. |

## ideatek:InputUser

`ideatek:InputUser` tells the user of a device.
`ideatek_input_user_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `uint32_t` | 0 | The user, from 1. 0 tells that the device has no user. |

## ideatek:DeviceState

`ideatek:DeviceState` tells the player and the battery of a device.
`ideatek_device_state_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `player_index` | `int32_t` | 0 | The player index of a joystick, or -1. |
| `percent` | `int32_t` | 4 | The charge of the battery, from 0 to 100, or -1. |
| `power` | `ideatek_power_t` | 8 | The state of the battery. |
| `link` | `ideatek_link_t` | 9 | How the device connects. |

Bytes 10 and 11 hold no field.

### ideatek:ideatek_power_t

`ideatek_power_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The battery |
| --- | --- | --- |
| `IDEATEK_POWER_UNKNOWN` | 0 | Not known. |
| `IDEATEK_POWER_ON_BATTERY` | 1 | The battery gives the power. |
| `IDEATEK_POWER_NO_BATTERY` | 2 | The device has no battery. |
| `IDEATEK_POWER_CHARGING` | 3 | The battery charges. |
| `IDEATEK_POWER_CHARGED` | 4 | The battery is full. |

### ideatek:ideatek_link_t

`ideatek_link_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The link |
| --- | --- | --- |
| `IDEATEK_LINK_UNKNOWN` | 0 | Not known. |
| `IDEATEK_LINK_WIRED` | 1 | A wire. |
| `IDEATEK_LINK_WIRELESS` | 2 | No wire. |

## ideatek:InputEvent

`ideatek:InputEvent` tells a change of a device or of the focus.
`ideatek_input_event_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `device` | `ideatek_entity_t` | 0 | The device entity, or 0 for a change of the focus. |
| `window` | `ideatek_entity_t` | 8 | With a change of the focus: the window. Else 0. |
| `user` | `uint32_t` | 16 | With `IDEATEK_INPUT_EVENT_KIND_USER`: the new user. |
| `old_user` | `uint32_t` | 20 | With `IDEATEK_INPUT_EVENT_KIND_USER`: the last user. |
| `order` | `uint32_t` | 24 | The rank of the event: *The events*. |
| `kind` | `ideatek_input_event_kind_t` | 28 | What changed. |

Bytes 29 to 31 hold no field.

### ideatek:ideatek_input_event_kind_t

`ideatek_input_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The change |
| --- | --- | --- |
| `IDEATEK_INPUT_EVENT_KIND_CONNECTED` | 0 | The device connected. Its entity is new. |
| `IDEATEK_INPUT_EVENT_KIND_DISCONNECTED` | 1 | The device disconnected. Its entity is destroyed. |
| `IDEATEK_INPUT_EVENT_KIND_POWER` | 2 | The battery changed. |
| `IDEATEK_INPUT_EVENT_KIND_USER` | 3 | The user of the device changed. |
| `IDEATEK_INPUT_EVENT_KIND_REMAPPED` | 4 | The mapping of the gamepad changed. |
| `IDEATEK_INPUT_EVENT_KIND_LAYOUT` | 5 | The layout of the keyboards changed. |
| `IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED` | 6 | The window got the keyboard focus. |
| `IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST` | 7 | The window lost the keyboard focus. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_device_kind_t` | `ideatek_device_kind_t` | `ideatek_device_kind_t_desc` | `IDEATEK_DEVICE_KIND_VALUES` |
| `ideatek:ideatek_power_t` | `ideatek_power_t` | `ideatek_power_t_desc` | `IDEATEK_POWER_VALUES` |
| `ideatek:ideatek_link_t` | `ideatek_link_t` | `ideatek_link_t_desc` | `IDEATEK_LINK_VALUES` |
| `ideatek:ideatek_input_event_kind_t` | `ideatek_input_event_kind_t` | `ideatek_input_event_kind_t_desc` | `IDEATEK_INPUT_EVENT_KIND_VALUES` |
| `ideatek:InputDevice` | `ideatek_input_device_t` | `ideatek_input_device_t_desc` | `IDEATEK_INPUT_DEVICE_FIELDS` |
| `ideatek:InputUser` | `ideatek_input_user_t` | `ideatek_input_user_t_desc` | `IDEATEK_INPUT_USER_FIELDS` |
| `ideatek:DeviceState` | `ideatek_device_state_t` | `ideatek_device_state_t_desc` | `IDEATEK_DEVICE_STATE_FIELDS` |
| `ideatek:InputEvent` | `ideatek_input_event_t` | `ideatek_input_event_t_desc` | `IDEATEK_INPUT_EVENT_FIELDS` |

```text
{ IDEATEK_DEVICE_KIND_KEYBOARD = 0, IDEATEK_DEVICE_KIND_MOUSE = 1, IDEATEK_DEVICE_KIND_JOYSTICK = 2, IDEATEK_DEVICE_KIND_TOUCH = 3, IDEATEK_DEVICE_KIND_PEN = 4, IDEATEK_DEVICE_KIND_SENSOR = 5 }
{ IDEATEK_POWER_UNKNOWN = 0, IDEATEK_POWER_ON_BATTERY = 1, IDEATEK_POWER_NO_BATTERY = 2, IDEATEK_POWER_CHARGING = 3, IDEATEK_POWER_CHARGED = 4 }
{ IDEATEK_LINK_UNKNOWN = 0, IDEATEK_LINK_WIRED = 1, IDEATEK_LINK_WIRELESS = 2 }
{ IDEATEK_INPUT_EVENT_KIND_CONNECTED = 0, IDEATEK_INPUT_EVENT_KIND_DISCONNECTED = 1, IDEATEK_INPUT_EVENT_KIND_POWER = 2, IDEATEK_INPUT_EVENT_KIND_USER = 3, IDEATEK_INPUT_EVENT_KIND_REMAPPED = 4, IDEATEK_INPUT_EVENT_KIND_LAYOUT = 5, IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED = 6, IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST = 7 }
{ bridge_text_t name; bridge_text_t serial; uint16_t vendor; uint16_t product; uint16_t version; uint8_t guid[16]; ideatek_device_kind_t kind; bool rumble; bool trigger_rumble; bool mono_led; bool rgb_led; bool player_led; }
{ uint32_t user; }
{ int32_t player_index; int32_t percent; ideatek_power_t power; ideatek_link_t link; }
{ ideatek_entity_t device; ideatek_entity_t window; uint32_t user; uint32_t old_user; uint32_t order; ideatek_input_event_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_device_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_power_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_link_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_input_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputUser` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DeviceState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

The field types of a header are `ideatek:ideatek_entity_t` and the enums and the bitmasks of the header.
A module declares the field types before the structs.
`bridge_text_t` belongs to Bridge, so no module declares it.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:InputDevice`, `ideatek:InputUser`, `ideatek:DeviceState` and `ideatek:InputEvent` | The field types |
| A reader | No type. | Its types and the field types |
