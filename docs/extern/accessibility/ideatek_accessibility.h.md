# include/accessibility/ideatek_accessibility.h

`include/accessibility/ideatek_accessibility.h` gives the types of the accessibility part: the preferences of the OS and the speech.
`readme.md`, *The contracts*, tells the rules of each contract.

## The roles

- The accessibility module writes `ideatek:AccessibilityState`, and reads `ideatek:SpeechRequest`. A deployment has one accessibility module.
- The game writes `ideatek:SpeechRequest`.
- A reader reads `ideatek:AccessibilityState`.

The OS gives no value for the color filters, the subtitles and the size of the text. The game keeps them in its own settings.
The speech of the chat between players is a concern of the game and of the audio part.

Comes back: the voice and its rate, when a menu shows them; a tree of nodes, when a game needs the screen reader.

## The preferences

At `init`, the accessibility module spawns 1 entity and attaches `ideatek:AccessibilityState` to it.
When the OS tells a change of a preference, the accessibility module attaches `ideatek:AccessibilityState` again in the same round.
An observer of `BRIDGE_ON_SET` tells a reader of the change.

A preference of the type `int8_t` has 1 of 3 values.
The value 1 tells that the preference is on, and the value 0 tells that it is off.
The value -1 tells that the OS gives no value, and the game uses its own setting.

## The speech

The game speaks a text when it attaches `ideatek:SpeechRequest` to an entity.
The accessibility module speaks the text each time the game writes the request, with the default voice of the OS.
When `speech` is false, the accessibility module writes 1 line in its log for a request, and speaks nothing.

At `init`, the accessibility module takes `text_bytes` bytes of its persistent arena.
It copies the text of a request there, with an end byte, because the OS needs the end byte.
For a text that is too large, it writes 1 line in its log and speaks nothing.
The `persistent` of its manifest includes these bytes.

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `text_bytes` | 4096 | 1 to 1073741824 | The bytes for the copy of the text of a request. |

## ideatek:AccessibilityState

`ideatek:AccessibilityState` gives the preferences of the OS for accessibility.
`ideatek_accessibility_state_t` takes 6 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `theme` | `ideatek_theme_t` | 0 | The theme of the OS. |
| `increase_contrast` | `int8_t` | 1 | The OS asks for more contrast. |
| `reduce_animation` | `int8_t` | 2 | The OS asks for less animation. |
| `reduce_transparency` | `int8_t` | 3 | The OS asks for less transparency. |
| `screen_reader` | `int8_t` | 4 | A screen reader runs. |
| `speech` | `bool` | 5 | The OS has a voice for the speech. |

### ideatek:ideatek_theme_t

`ideatek_theme_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The theme |
| --- | --- | --- |
| `IDEATEK_THEME_UNKNOWN` | 0 | The OS gives no theme. |
| `IDEATEK_THEME_LIGHT` | 1 | A light theme. |
| `IDEATEK_THEME_DARK` | 2 | A dark theme. |

## ideatek:SpeechRequest

`ideatek:SpeechRequest` asks to speak a text.
`ideatek_speech_request_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The text in UTF-8. The game writes it in its share. |
| `interrupt` | `bool` | 8 | True stops the speech before, and false puts the text after it. |

Bytes 9 to 11 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_theme_t` | `ideatek_theme_t` | `ideatek_theme_t_desc` | `IDEATEK_THEME_VALUES` |
| `ideatek:AccessibilityState` | `ideatek_accessibility_state_t` | `ideatek_accessibility_state_t_desc` | `IDEATEK_ACCESSIBILITY_STATE_FIELDS` |
| `ideatek:SpeechRequest` | `ideatek_speech_request_t` | `ideatek_speech_request_t_desc` | `IDEATEK_SPEECH_REQUEST_FIELDS` |

```text
{ IDEATEK_THEME_UNKNOWN = 0, IDEATEK_THEME_LIGHT = 1, IDEATEK_THEME_DARK = 2 }
{ ideatek_theme_t theme; int8_t increase_contrast; int8_t reduce_animation; int8_t reduce_transparency; int8_t screen_reader; bool speech; }
{ bridge_text_t text; bool interrupt; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_theme_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AccessibilityState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SpeechRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares `ideatek:ideatek_theme_t` before `ideatek:AccessibilityState`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:SpeechRequest` | No type. |
| The accessibility module | `ideatek:AccessibilityState` | `ideatek:ideatek_theme_t` and `ideatek:SpeechRequest` |
| A reader | No type. | `ideatek:ideatek_theme_t` and `ideatek:AccessibilityState` |
