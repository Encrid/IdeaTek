# include/platform_abstraction/input/ideatek_keyboard.h

`include/platform_abstraction/input/ideatek_keyboard.h` gives the types of the keyboard.
`docs/extern/platform_abstraction/input/ideatek_input.h.md` tells the common rules.

## The codes of a key

Each key has 2 codes:

- `code` is the physical key: its USB usage of the page of keyboards, below 512. The layout does not change it.
- `key` is the logical key in the layout.

The logical key of a character is its Unicode code point.
Each other logical key is its `code` with the bit 30 set.
A key with no code point and no USB usage has `key` 0, and the input module does not send it.

## ideatek:Keyboard

`ideatek:Keyboard` keeps the keys that are down.
`ideatek_keyboard_t` takes 72 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `keys` | `uint64_t[8]` | 0 | 512 bits: the bit `code % 64` of `keys[code / 64]` is 1 while the key is down. |
| `modifiers` | `ideatek_modifier_t` | 64 | The modifiers that are on. |

Bytes 66 to 71 hold no field.

### ideatek:ideatek_modifier_t

`ideatek_modifier_t` is a bitmask of `uint16_t`. Its size and its alignment are 2.

| Constant | Value | The modifier |
| --- | --- | --- |
| `IDEATEK_MODIFIER_LSHIFT` | 0x0001 | The left shift. |
| `IDEATEK_MODIFIER_RSHIFT` | 0x0002 | The right shift. |
| `IDEATEK_MODIFIER_LEVEL5` | 0x0004 | The shift of the level 5. |
| `IDEATEK_MODIFIER_LCTRL` | 0x0040 | The left control. |
| `IDEATEK_MODIFIER_RCTRL` | 0x0080 | The right control. |
| `IDEATEK_MODIFIER_LALT` | 0x0100 | The left alt. |
| `IDEATEK_MODIFIER_RALT` | 0x0200 | The right alt. |
| `IDEATEK_MODIFIER_LGUI` | 0x0400 | The left key of the system. |
| `IDEATEK_MODIFIER_RGUI` | 0x0800 | The right key of the system. |
| `IDEATEK_MODIFIER_NUM` | 0x1000 | The num lock. |
| `IDEATEK_MODIFIER_CAPS` | 0x2000 | The caps lock. |
| `IDEATEK_MODIFIER_MODE` | 0x4000 | The key of the mode, AltGr. |
| `IDEATEK_MODIFIER_SCROLL` | 0x8000 | The scroll lock. |

## ideatek:KeyEvent

`ideatek:KeyEvent` tells that a key goes down or up.
`ideatek_key_event_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `key` | `uint32_t` | 0 | The logical key. |
| `order` | `uint32_t` | 4 | The rank of the event: `docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The events*. |
| `code` | `uint16_t` | 8 | The physical key. |
| `modifiers` | `ideatek_modifier_t` | 10 | The modifiers at the time of the event. |
| `down` | `bool` | 12 | `true` when the key goes down. |
| `repeat` | `bool` | 13 | `true` when the system repeats a key that stays down. |

Bytes 14 and 15 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_modifier_t` | `ideatek_modifier_t` | `ideatek_modifier_t_desc` | `IDEATEK_MODIFIER_VALUES` |
| `ideatek:Keyboard` | `ideatek_keyboard_t` | `ideatek_keyboard_t_desc` | `IDEATEK_KEYBOARD_FIELDS` |
| `ideatek:KeyEvent` | `ideatek_key_event_t` | `ideatek_key_event_t_desc` | `IDEATEK_KEY_EVENT_FIELDS` |

```text
{ IDEATEK_MODIFIER_LSHIFT = 0x0001, IDEATEK_MODIFIER_RSHIFT = 0x0002, IDEATEK_MODIFIER_LEVEL5 = 0x0004, IDEATEK_MODIFIER_LCTRL = 0x0040, IDEATEK_MODIFIER_RCTRL = 0x0080, IDEATEK_MODIFIER_LALT = 0x0100, IDEATEK_MODIFIER_RALT = 0x0200, IDEATEK_MODIFIER_LGUI = 0x0400, IDEATEK_MODIFIER_RGUI = 0x0800, IDEATEK_MODIFIER_NUM = 0x1000, IDEATEK_MODIFIER_CAPS = 0x2000, IDEATEK_MODIFIER_MODE = 0x4000, IDEATEK_MODIFIER_SCROLL = 0x8000 }
{ uint64_t keys[8]; ideatek_modifier_t modifiers; }
{ uint32_t key; uint32_t order; uint16_t code; ideatek_modifier_t modifiers; bool down; bool repeat; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_modifier_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Keyboard` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:KeyEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:Keyboard` and `ideatek:KeyEvent` | The field types |
| A reader | No type. | Its types and the field types |
