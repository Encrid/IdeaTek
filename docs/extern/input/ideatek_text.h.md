# include/input/ideatek_text.h

`include/input/ideatek_text.h` gives the types of the text input.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The text input

The game attaches `ideatek:TextRequest` to a window entity.
With `enabled` at `true`, the input module starts the text input of the window.
Then it spawns an `ideatek:TextEvent` for each text of the user.

A commit is a text that the user ended. A composition is a text that the input method still changes.
Each candidate of the input method is 1 event: `index` tells which, and `count` tells how many.
A list of 0 candidates is 1 event with `count` at 0.

With `draws_composition` or `draws_candidates`, the game draws them. Else the system draws them.
The system reads these 2 fields only when the text input starts.

## ideatek:TextRequest

`ideatek:TextRequest` asks for the text input of a window.
`ideatek_text_request_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window that gets the text. |
| `x` | `int32_t` | 8 | The left side of the area of the text, in the window. |
| `y` | `int32_t` | 12 | The top side of the area of the text. |
| `width` | `uint32_t` | 16 | The width of the area of the text. |
| `height` | `uint32_t` | 20 | The height of the area of the text. |
| `cursor` | `int32_t` | 24 | The distance of the cursor from `x`, in pixels. |
| `clears` | `uint32_t` | 28 | A count: each addition of 1 clears the composition. |
| `type` | `ideatek_text_type_t` | 32 | The kind of the text. |
| `capitalize` | `ideatek_capitalize_t` | 33 | The capitals of the keyboard on the screen. |
| `enabled` | `bool` | 34 | `true` starts the text input. `false` stops it. |
| `autocorrect` | `bool` | 35 | `true` lets the system correct the words. |
| `multiline` | `bool` | 36 | `true` lets the text hold more than 1 line. |
| `draws_composition` | `bool` | 37 | `true` if the game draws the composition. |
| `draws_candidates` | `bool` | 38 | `true` if the game draws the candidates. |

Byte 39 holds no field.

### ideatek:ideatek_text_type_t

`ideatek_text_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The text |
| --- | --- | --- |
| `IDEATEK_TEXT_TYPE_TEXT` | 0 | Any text. |
| `IDEATEK_TEXT_TYPE_TEXT_NAME` | 1 | A name. |
| `IDEATEK_TEXT_TYPE_TEXT_EMAIL` | 2 | An email address. |
| `IDEATEK_TEXT_TYPE_TEXT_USERNAME` | 3 | A user name. |
| `IDEATEK_TEXT_TYPE_TEXT_PASSWORD_HIDDEN` | 4 | A password that the system hides. |
| `IDEATEK_TEXT_TYPE_TEXT_PASSWORD_VISIBLE` | 5 | A password that the user sees. |
| `IDEATEK_TEXT_TYPE_NUMBER` | 6 | A number. |
| `IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_HIDDEN` | 7 | A code of digits that the system hides. |
| `IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_VISIBLE` | 8 | A code of digits that the user sees. |

### ideatek:ideatek_capitalize_t

`ideatek_capitalize_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The capitals |
| --- | --- | --- |
| `IDEATEK_CAPITALIZE_NONE` | 0 | No capitals. |
| `IDEATEK_CAPITALIZE_SENTENCES` | 1 | The first letter of each sentence. |
| `IDEATEK_CAPITALIZE_WORDS` | 2 | The first letter of each word. |
| `IDEATEK_CAPITALIZE_LETTERS` | 3 | Each letter. |

## ideatek:TextEvent

`ideatek:TextEvent` gives a text of the user.
`ideatek_text_event_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `keyboard` | `ideatek_entity_t` | 0 | The keyboard entity, or 0. |
| `window` | `ideatek_entity_t` | 8 | The window of the text. |
| `text` | `bridge_text_t` | 16 | The text, in UTF-8, in `text_bytes`. |
| `start` | `int32_t` | 24 | With a composition: the first selected character, or -1. |
| `length` | `int32_t` | 28 | With a composition: the number of selected characters, or -1. |
| `index` | `uint32_t` | 32 | With a candidate: its rank, from 0. |
| `count` | `uint32_t` | 36 | With a candidate: the number of candidates. |
| `selected` | `int32_t` | 40 | With a candidate: the rank of the selected candidate, or -1. |
| `order` | `uint32_t` | 44 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |
| `kind` | `ideatek_text_event_kind_t` | 48 | What the text is. |
| `horizontal` | `bool` | 49 | With a candidate: `true` if the list is horizontal. |

Bytes 50 to 55 hold no field.

### ideatek:ideatek_text_event_kind_t

`ideatek_text_event_kind_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The text |
| --- | --- | --- |
| `IDEATEK_TEXT_EVENT_KIND_COMMIT` | 0 | A text that the user ended. |
| `IDEATEK_TEXT_EVENT_KIND_COMPOSITION` | 1 | A text that the input method changes. |
| `IDEATEK_TEXT_EVENT_KIND_CANDIDATE` | 2 | A candidate of the input method. |
| `IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_SHOWN` | 3 | The keyboard on the screen shows. `text` has no bytes. |
| `IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_HIDDEN` | 4 | The keyboard on the screen hides. `text` has no bytes. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_text_type_t` | `ideatek_text_type_t` | `ideatek_text_type_t_desc` | `IDEATEK_TEXT_TYPE_VALUES` |
| `ideatek:ideatek_capitalize_t` | `ideatek_capitalize_t` | `ideatek_capitalize_t_desc` | `IDEATEK_CAPITALIZE_VALUES` |
| `ideatek:ideatek_text_event_kind_t` | `ideatek_text_event_kind_t` | `ideatek_text_event_kind_t_desc` | `IDEATEK_TEXT_EVENT_KIND_VALUES` |
| `ideatek:TextRequest` | `ideatek_text_request_t` | `ideatek_text_request_t_desc` | `IDEATEK_TEXT_REQUEST_FIELDS` |
| `ideatek:TextEvent` | `ideatek_text_event_t` | `ideatek_text_event_t_desc` | `IDEATEK_TEXT_EVENT_FIELDS` |

```text
{ IDEATEK_TEXT_TYPE_TEXT = 0, IDEATEK_TEXT_TYPE_TEXT_NAME = 1, IDEATEK_TEXT_TYPE_TEXT_EMAIL = 2, IDEATEK_TEXT_TYPE_TEXT_USERNAME = 3, IDEATEK_TEXT_TYPE_TEXT_PASSWORD_HIDDEN = 4, IDEATEK_TEXT_TYPE_TEXT_PASSWORD_VISIBLE = 5, IDEATEK_TEXT_TYPE_NUMBER = 6, IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_HIDDEN = 7, IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_VISIBLE = 8 }
{ IDEATEK_CAPITALIZE_NONE = 0, IDEATEK_CAPITALIZE_SENTENCES = 1, IDEATEK_CAPITALIZE_WORDS = 2, IDEATEK_CAPITALIZE_LETTERS = 3 }
{ IDEATEK_TEXT_EVENT_KIND_COMMIT = 0, IDEATEK_TEXT_EVENT_KIND_COMPOSITION = 1, IDEATEK_TEXT_EVENT_KIND_CANDIDATE = 2, IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_SHOWN = 3, IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_HIDDEN = 4 }
{ ideatek_entity_t window; int32_t x; int32_t y; uint32_t width; uint32_t height; int32_t cursor; uint32_t clears; ideatek_text_type_t type; ideatek_capitalize_t capitalize; bool enabled; bool autocorrect; bool multiline; bool draws_composition; bool draws_candidates; }
{ ideatek_entity_t keyboard; ideatek_entity_t window; bridge_text_t text; int32_t start; int32_t length; uint32_t index; uint32_t count; int32_t selected; uint32_t order; ideatek_text_event_kind_t kind; bool horizontal; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_capitalize_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TextRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TextEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:TextRequest` | The field types |
| The input module | `ideatek:TextEvent` | `ideatek:TextRequest` and the field types |
| A reader | No type. | Its types and the field types |
