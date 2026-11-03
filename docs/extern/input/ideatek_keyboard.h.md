# include/input/ideatek_keyboard.h

`include/input/ideatek_keyboard.h` gives the types of the keyboards.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The codes of a key

Each key has 3 codes:

- `code` is the physical key: its USB usage of the page of keyboards, below 512. The layout does not change it.
- `key` is the logical key in the layout: the key code of SDL 3.
- `raw` is the code of the system, or 0.

The logical key of a character is its Unicode code point.
Each other logical key is its `code` with the bit 30 set.

## The layout

When the layout changes, the input module attaches `ideatek:KeyLayout` again.
It also spawns an `ideatek:InputEvent` with `IDEATEK_INPUT_EVENT_KIND_LAYOUT`.

## ideatek:Keyboard

`ideatek:Keyboard` holds the keys that are down.
`ideatek_keyboard_t` takes 80 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window that has the keyboard focus, or 0. |
| `keys` | `uint64_t[8]` | 8 | 512 bits: the bit `code % 64` of `keys[code / 64]` is 1 while the key is down. |
| `modifiers` | `ideatek_modifier_t` | 72 | The modifiers that are on. |
| `ime` | `bool` | 74 | `true` if the system has an input method. |
| `screen_keyboard` | `bool` | 75 | `true` if the system has a keyboard on the screen. |

Bytes 76 to 79 hold no field.

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

## ideatek:KeyLayout

`ideatek:KeyLayout` gives the logical key of each physical key, in the current layout.
`ideatek_key_layout_t` takes 2048 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `key` | `uint32_t[512]` | 0 | `key[code]` is the logical key of `code`, with no modifier. |

## ideatek:KeyEvent

`ideatek:KeyEvent` tells that a key went down or up.
`ideatek_key_event_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `keyboard` | `ideatek_entity_t` | 0 | The keyboard entity. |
| `window` | `ideatek_entity_t` | 8 | The window that has the focus, or 0. |
| `key` | `uint32_t` | 16 | The logical key. |
| `raw` | `uint32_t` | 20 | The code of the system, or 0. |
| `order` | `uint32_t` | 24 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `code` | `uint16_t` | 28 | The physical key. |
| `modifiers` | `ideatek_modifier_t` | 30 | The modifiers at the time of the event. |
| `down` | `bool` | 32 | `true` when the key goes down. |
| `repeat` | `bool` | 33 | `true` when the system repeats a key that stays down. |

Bytes 34 to 39 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_modifier_t` | `ideatek_modifier_t` | `ideatek_modifier_t_desc` | `IDEATEK_MODIFIER_VALUES` |
| `ideatek:Keyboard` | `ideatek_keyboard_t` | `ideatek_keyboard_t_desc` | `IDEATEK_KEYBOARD_FIELDS` |
| `ideatek:KeyLayout` | `ideatek_key_layout_t` | `ideatek_key_layout_t_desc` | `IDEATEK_KEY_LAYOUT_FIELDS` |
| `ideatek:KeyEvent` | `ideatek_key_event_t` | `ideatek_key_event_t_desc` | `IDEATEK_KEY_EVENT_FIELDS` |

```text
{ IDEATEK_MODIFIER_LSHIFT = 0x0001, IDEATEK_MODIFIER_RSHIFT = 0x0002, IDEATEK_MODIFIER_LEVEL5 = 0x0004, IDEATEK_MODIFIER_LCTRL = 0x0040, IDEATEK_MODIFIER_RCTRL = 0x0080, IDEATEK_MODIFIER_LALT = 0x0100, IDEATEK_MODIFIER_RALT = 0x0200, IDEATEK_MODIFIER_LGUI = 0x0400, IDEATEK_MODIFIER_RGUI = 0x0800, IDEATEK_MODIFIER_NUM = 0x1000, IDEATEK_MODIFIER_CAPS = 0x2000, IDEATEK_MODIFIER_MODE = 0x4000, IDEATEK_MODIFIER_SCROLL = 0x8000 }
{ ideatek_entity_t window; uint64_t keys[8]; ideatek_modifier_t modifiers; bool ime; bool screen_keyboard; }
{ uint32_t key[512]; }
{ ideatek_entity_t keyboard; ideatek_entity_t window; uint32_t key; uint32_t raw; uint32_t order; uint16_t code; ideatek_modifier_t modifiers; bool down; bool repeat; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_modifier_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Keyboard` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:KeyLayout` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:KeyEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | No type. | The field types |
| The input module | `ideatek:Keyboard`, `ideatek:KeyLayout` and `ideatek:KeyEvent` | The field types |
| A reader | No type. | Its types and the field types |
