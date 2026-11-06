# src/platform_abstraction/input_sdl3/input_usage.c

This file gives the usage of the USB HID Usage Tables of a key, from the scancode and the keycode of SDL3.
The contract tells the usages: `docs/extern/platform_abstraction/ideatek_input_keyboard.h.md`, *The codes of a key*.

## The physical key

`usage_of_scancode` gives `code` from the scancode of SDL3:

- A scancode from 4 to 231 is the id of the same key on the page 0x07. SDL3 takes its scancodes from that page: https://github.com/libsdl-org/SDL/blob/release-3.4.20/include/SDL3/SDL_scancode.h.
- A scancode from 258 to 286 has a row in *The keys of the other pages*.
- Each other scancode gives 0. Examples are `SDL_SCANCODE_MODE` and the keys of the phones from 287. Such a key has no event.

## The keys of the other pages

| Scancode of SDL3 | Page | Id | Name of the usage |
| --- | --- | --- | --- |
| `SDL_SCANCODE_SLEEP` | 0x0C | 0x32 | Sleep |
| `SDL_SCANCODE_WAKE` | 0x01 | 0x83 | System Wake Up |
| `SDL_SCANCODE_CHANNEL_INCREMENT` | 0x0C | 0x9C | Channel Increment |
| `SDL_SCANCODE_CHANNEL_DECREMENT` | 0x0C | 0x9D | Channel Decrement |
| `SDL_SCANCODE_MEDIA_PLAY` | 0x0C | 0xB0 | Play |
| `SDL_SCANCODE_MEDIA_PAUSE` | 0x0C | 0xB1 | Pause |
| `SDL_SCANCODE_MEDIA_RECORD` | 0x0C | 0xB2 | Record |
| `SDL_SCANCODE_MEDIA_FAST_FORWARD` | 0x0C | 0xB3 | Fast Forward |
| `SDL_SCANCODE_MEDIA_REWIND` | 0x0C | 0xB4 | Rewind |
| `SDL_SCANCODE_MEDIA_NEXT_TRACK` | 0x0C | 0xB5 | Scan Next Track |
| `SDL_SCANCODE_MEDIA_PREVIOUS_TRACK` | 0x0C | 0xB6 | Scan Previous Track |
| `SDL_SCANCODE_MEDIA_STOP` | 0x0C | 0xB7 | Stop |
| `SDL_SCANCODE_MEDIA_EJECT` | 0x0C | 0xB8 | Eject |
| `SDL_SCANCODE_MEDIA_PLAY_PAUSE` | 0x0C | 0xCD | Play/Pause |
| `SDL_SCANCODE_MEDIA_SELECT` | 0x0C | 0x87 | Media Selection |
| `SDL_SCANCODE_AC_NEW` | 0x0C | 0x201 | AC New |
| `SDL_SCANCODE_AC_OPEN` | 0x0C | 0x202 | AC Open |
| `SDL_SCANCODE_AC_CLOSE` | 0x0C | 0x203 | AC Close |
| `SDL_SCANCODE_AC_EXIT` | 0x0C | 0x204 | AC Exit |
| `SDL_SCANCODE_AC_SAVE` | 0x0C | 0x207 | AC Save |
| `SDL_SCANCODE_AC_PRINT` | 0x0C | 0x208 | AC Print |
| `SDL_SCANCODE_AC_PROPERTIES` | 0x0C | 0x209 | AC Properties |
| `SDL_SCANCODE_AC_SEARCH` | 0x0C | 0x221 | AC Search |
| `SDL_SCANCODE_AC_HOME` | 0x0C | 0x223 | AC Home |
| `SDL_SCANCODE_AC_BACK` | 0x0C | 0x224 | AC Back |
| `SDL_SCANCODE_AC_FORWARD` | 0x0C | 0x225 | AC Forward |
| `SDL_SCANCODE_AC_STOP` | 0x0C | 0x226 | AC Stop |
| `SDL_SCANCODE_AC_REFRESH` | 0x0C | 0x227 | AC Refresh |
| `SDL_SCANCODE_AC_BOOKMARKS` | 0x0C | 0x22A | AC Bookmarks |

The ids come from the HID Usage Tables, as these 2 copies give them: https://www.freebsddiary.org/APC/usb_hid_usages.php and https://www.usbzh.com/article/detail-982.html.
SDL3 groups the sleep and the wake with the page 0x0C. The page 0x0C has a usage of the sleep, and no usage of the wake, so the wake takes its usage of the page 0x01.

## The logical key

`usage_of_keycode` gives `key` from the keycode of the event:

- A keycode with `SDLK_SCANCODE_MASK` is a key with no character. It gives the usage of its scancode, as *The physical key* tells.
- A keycode with `SDLK_EXTENDED_MASK` gives 0.
- A keycode that is a character gives the usage of its key on the layout of the tables, the layout of the United States. *The characters* gives them.
- Each other character gives 0, because no usage names it.

Thus on a layout AZERTY, the key with the code of Q gives `key` of the usage of A.

## The characters

| Characters | Usages |
| --- | --- |
| `a` to `z` | 0x04 to 0x1D |
| `1` to `9`, then `0` | 0x1E to 0x27 |
| Return, Escape, Backspace, Tab, Space | 0x28, 0x29, 0x2A, 0x2B, 0x2C |
| `-` `=` `[` `]` `\` | 0x2D, 0x2E, 0x2F, 0x30, 0x31 |
| `;` `'` `` ` `` `,` `.` `/` | 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 |
| Delete | 0x4C |

Each usage is on the page 0x07.

## The character

`character_of` gives `character` from `SDL_GetKeyFromScancode` with the modifiers of the event and `key_event` at `false`.
The modifiers give the shifted character, for example `A` with Shift. A keycode with a mask, a control character below 0x20, and Delete give 0.
`key_event` at `false` keeps the keycode out of the options of `SDL_HINT_KEYCODE_OPTIONS`, so the character is the character of the layout: https://wiki.libsdl.org/SDL3/SDL_GetKeyFromScancode.

## The branches

| Case | Result |
| --- | --- |
| A scancode from 4 to 231. | The usage of the page 0x07 with the same id. |
| A scancode of *The keys of the other pages*. | Its usage of the page 0x0C or 0x01. |
| A scancode with no usage. | 0. |
| A keycode with `SDLK_SCANCODE_MASK`. | The usage of its scancode. |
| A keycode with `SDLK_EXTENDED_MASK`. | 0. |
| A character of *The characters*. | Its usage. |
| A character outside *The characters*. | 0. |
| A key whose character is printable. | Its code point. |
| A key whose character is a control character, or that has no character. | 0. |
| A scancode below 4, or from 232 to 257. | 0. |
| The characters `1`, `0`, `;` and Delete. | Their usages of *The characters*. |
| A keycode of 0. | 0. |

## Functions

| Function | Result |
| --- | --- |
| `usage_of_scancode` | Gives the usage of the physical key, or 0. |
| `usage_of_character` | Gives the usage of the key of a character on the layout of the tables, or 0. |
| `usage_of_keycode` | Gives the usage of the logical key, or 0. |
| `character_of` | Gives the code point of a key with its modifiers, or 0. |
