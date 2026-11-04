# include/platform_abstraction/ideatek_input_keyboard.h

`include/platform_abstraction/ideatek_input_keyboard.h` gives the types of the keyboard.
`docs/extern/platform_abstraction/ideatek_input.h.md` tells the common rules.

## The codes of a key

Each key has 3 values:

- `code` is the physical key. The layout and the remaps of the system do not change it.
- `key` is the logical key: the key after the layout and after the remaps of the system.
- `character` is the Unicode code point that the layout gives to the key.

`code` and `key` are usages of the USB HID Usage Tables, with the value `(page << 16) | id`.
For example, the key A has the value 0x00070004 on the page 0x07.
The input module sends only keys of the page 0x07 of keyboards, the page 0x0C of consumers and the page 0x01 of the generic desktop.
`key` is 0 when the system gives no logical key.
`character` is 0 when the key gives no character.
A key with no usage on these pages has no event.

## ideatek:Keyboard

`ideatek:Keyboard` keeps the keys that are down.
`ideatek_keyboard_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `keys` | `uint64_t[4]` | 0 | 256 bits, 1 for each id of the page 0x07: the bit `id % 64` of `keys[id / 64]` is 1 while the key is down. |
| `modifiers` | `ideatek_modifier_t` | 32 | The modifiers that are on. |

Bytes 34 to 39 hold no field.
A key of the page 0x0C or of the page 0x01 has events, but no bit in `keys`.

### ideatek:ideatek_modifier_t

`ideatek_modifier_t` is a bitmask of `uint16_t`. Its size and its alignment are 2.

The bits 0 to 7 follow the modifier byte of the boot report of the USB HID class. The bits 8 to 10 follow the LEDs of the page 0x08. The mask is the state that the OS gives after its remaps and its sticky keys. `keys` is the physical state.

| Constant | Value | The modifier |
| --- | --- | --- |
| `IDEATEK_MODIFIER_LCTRL` | 0x0001 | The left control. |
| `IDEATEK_MODIFIER_LSHIFT` | 0x0002 | The left shift. |
| `IDEATEK_MODIFIER_LALT` | 0x0004 | The left alt. |
| `IDEATEK_MODIFIER_LGUI` | 0x0008 | The left key of the system. |
| `IDEATEK_MODIFIER_RCTRL` | 0x0010 | The right control. |
| `IDEATEK_MODIFIER_RSHIFT` | 0x0020 | The right shift. |
| `IDEATEK_MODIFIER_RALT` | 0x0040 | The right alt. |
| `IDEATEK_MODIFIER_RGUI` | 0x0080 | The right key of the system. |
| `IDEATEK_MODIFIER_NUM` | 0x0100 | The num lock. |
| `IDEATEK_MODIFIER_CAPS` | 0x0200 | The caps lock. |
| `IDEATEK_MODIFIER_SCROLL` | 0x0400 | The scroll lock. |

## ideatek:KeyEvent

`ideatek:KeyEvent` tells that a key goes down or up.
`ideatek_key_event_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `character` | `uint32_t` | 0 | The character of the key. |
| `key` | `uint32_t` | 4 | The logical key. |
| `code` | `uint32_t` | 8 | The physical key. |
| `order` | `uint32_t` | 12 | The rank of the event: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The events*. |
| `modifiers` | `ideatek_modifier_t` | 16 | The modifiers at the time of the event. |
| `down` | `bool` | 18 | `true` when the key goes down. |
| `repeat` | `bool` | 19 | `true` when the system repeats a key that stays down. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_modifier_t` | `ideatek_modifier_t` | `ideatek_modifier_t_desc` | `IDEATEK_MODIFIER_VALUES` |
| `ideatek:Keyboard` | `ideatek_keyboard_t` | `ideatek_keyboard_t_desc` | `IDEATEK_KEYBOARD_FIELDS` |
| `ideatek:KeyEvent` | `ideatek_key_event_t` | `ideatek_key_event_t_desc` | `IDEATEK_KEY_EVENT_FIELDS` |

```text
{ IDEATEK_MODIFIER_LCTRL = 0x0001, IDEATEK_MODIFIER_LSHIFT = 0x0002, IDEATEK_MODIFIER_LALT = 0x0004, IDEATEK_MODIFIER_LGUI = 0x0008, IDEATEK_MODIFIER_RCTRL = 0x0010, IDEATEK_MODIFIER_RSHIFT = 0x0020, IDEATEK_MODIFIER_RALT = 0x0040, IDEATEK_MODIFIER_RGUI = 0x0080, IDEATEK_MODIFIER_NUM = 0x0100, IDEATEK_MODIFIER_CAPS = 0x0200, IDEATEK_MODIFIER_SCROLL = 0x0400 }
{ uint64_t keys[4]; ideatek_modifier_t modifiers; }
{ uint32_t character; uint32_t key; uint32_t code; uint32_t order; ideatek_modifier_t modifiers; bool down; bool repeat; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_modifier_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Keyboard` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:KeyEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:Keyboard` and `ideatek:KeyEvent` | The field types |
| A reader | No type. | Its types and the field types |
