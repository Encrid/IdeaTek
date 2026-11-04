# include/platform/system/ideatek_system.h

`include/platform/system/ideatek_system.h` gives the types of the services of the system: the locales, the links and the message boxes.
`readme.md`, *The contracts*, tells the rules of each contract.

## The roles

- The system module writes `ideatek:SystemState`, and reads `ideatek:UrlRequest` and `ideatek:MessageRequest`. A deployment has one system module.
- The game writes `ideatek:UrlRequest` and `ideatek:MessageRequest`.
- A reader reads `ideatek:SystemState`.

Comes back: the power state with a mobile or handheld target; the file dialog when a game needs it.
Notifications and the tray do not come back, because a game does not use them.

## The locales

At `init`, the system module spawns 1 entity and attaches `ideatek:SystemState` to it.
When the system tells a change of the locales, the system module attaches `ideatek:SystemState` again in the same round.
A reader hears the change with an observer of `BRIDGE_ON_ATTACH`.

## The requests

The game opens a link in the browser of the system when it attaches `ideatek:UrlRequest` to an entity.
The system module opens the link each time the game writes the request.
The system module opens only a link with the scheme `http` or `https`. A link with another scheme gets 1 line in the log of the system module, and no action.

The game shows a message box when it attaches `ideatek:MessageRequest` to an entity.
The system module shows the box each time the game writes the request.
The box has 1 button, OK. The round waits until the player closes the box.
The game writes each text of a request in UTF-8, in its share.

## The bytes of the texts

The system module reserves `text_bytes` bytes of the persistent arena at `init`, in its share.
The locales use the first half of the bytes.
If the locales are too large for their half, `locales` keeps only the first locales that fit. The log of the system module gets 1 line.
The system module copies the texts of a request into the second half, each with an end byte, because the system needs the end byte.
A request whose texts are too large for the second half gets 1 line in the log, and no action.

The manifest of the system module gives `persistent`, which must hold `text_bytes` too.

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `text_bytes` | 4096 | 2 to 1073741824, an even number | The bytes for the locales and for the copy of a request. Each use has the half. |

## ideatek:SystemState

`ideatek:SystemState` gives the state of the system that a game shows.
`ideatek_system_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `locales` | `bridge_text_t` | 0 | The locales of the user, in UTF-8, in the order of preference, with a comma between 2 locales, as `fr-CA,en`. |

## ideatek:UrlRequest

`ideatek:UrlRequest` asks to open a link.
`ideatek_url_request_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `url` | `bridge_text_t` | 0 | The link. |

## ideatek:MessageRequest

`ideatek:MessageRequest` asks to show a message box.
`ideatek_message_request_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `title` | `bridge_text_t` | 0 | The title of the box. |
| `message` | `bridge_text_t` | 8 | The message. |
| `level` | `ideatek_message_level_t` | 16 | The level of the message. The system shows the icon of the level. |

Bytes 17 to 19 hold no field.

### ideatek:ideatek_message_level_t

`ideatek_message_level_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The level |
| --- | --- | --- |
| `IDEATEK_MESSAGE_LEVEL_ERROR` | 0 | An error. |
| `IDEATEK_MESSAGE_LEVEL_WARNING` | 1 | A warning. |
| `IDEATEK_MESSAGE_LEVEL_INFORMATION` | 2 | An information. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:SystemState` | `ideatek_system_state_t` | `ideatek_system_state_t_desc` | `IDEATEK_SYSTEM_STATE_FIELDS` |
| `ideatek:UrlRequest` | `ideatek_url_request_t` | `ideatek_url_request_t_desc` | `IDEATEK_URL_REQUEST_FIELDS` |
| `ideatek:ideatek_message_level_t` | `ideatek_message_level_t` | `ideatek_message_level_t_desc` | `IDEATEK_MESSAGE_LEVEL_VALUES` |
| `ideatek:MessageRequest` | `ideatek_message_request_t` | `ideatek_message_request_t_desc` | `IDEATEK_MESSAGE_REQUEST_FIELDS` |

```text
{ bridge_text_t locales; }
{ bridge_text_t url; }
{ IDEATEK_MESSAGE_LEVEL_ERROR = 0, IDEATEK_MESSAGE_LEVEL_WARNING = 1, IDEATEK_MESSAGE_LEVEL_INFORMATION = 2 }
{ bridge_text_t title; bridge_text_t message; ideatek_message_level_t level; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_message_level_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SystemState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UrlRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MessageRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares `ideatek:ideatek_message_level_t` before `ideatek:MessageRequest`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:UrlRequest` and `ideatek:MessageRequest` | `ideatek:ideatek_message_level_t` |
| The system module | `ideatek:SystemState` | `ideatek:ideatek_message_level_t`, `ideatek:UrlRequest` and `ideatek:MessageRequest` |
| A reader | No type. | `ideatek:SystemState` |
