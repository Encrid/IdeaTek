# include/platform_abstraction/ideatek_input_text.h

`include/platform_abstraction/ideatek_input_text.h` gives the types of the text input.
`docs/extern/platform_abstraction/ideatek_input.h.md` tells the common rules.

## The text input

The game attaches `ideatek:TextRequest` to the input entity, the entity of `ideatek:Keyboard`.
The text input runs while the input entity has `ideatek:TextRequest`.
When the game removes it, the input module stops the text input.
While the text input runs, the input module spawns an `ideatek:TextEvent` for each character that the user ends.
The OS draws the IME composition.

## ideatek:TextRequest

`ideatek:TextRequest` asks for the text input.
`ideatek_text_request_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `screen_keyboard` | `bool` | 0 | `true` shows the keyboard on the screen, on a system that has one. |

## ideatek:TextEvent

`ideatek:TextEvent` gives 1 character of the user.
`ideatek_text_event_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `code_point` | `uint32_t` | 0 | The Unicode code point of the character. |
| `order` | `uint32_t` | 4 | The rank of the event: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The events*. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:TextRequest` | `ideatek_text_request_t` | `ideatek_text_request_t_desc` | `IDEATEK_TEXT_REQUEST_FIELDS` |
| `ideatek:TextEvent` | `ideatek_text_event_t` | `ideatek_text_event_t_desc` | `IDEATEK_TEXT_EVENT_FIELDS` |

```text
{ bool screen_keyboard; }
{ uint32_t code_point; uint32_t order; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:TextRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TextEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`include/platform_abstraction/ideatek_input_text.h` has no field type.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:TextRequest` | No type. |
| The input module | `ideatek:TextEvent` | `ideatek:TextRequest` |
| A reader | No type. | Its types |
