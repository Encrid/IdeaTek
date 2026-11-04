# include/platform_abstraction/ideatek_window_clipboard.h

`include/platform_abstraction/ideatek_window_clipboard.h` gives the types of the clipboard.
`docs/extern/platform_abstraction/ideatek_window.h.md` tells the common rules.

## The clipboard

The game puts a text on the clipboard of the system when it attaches `ideatek:ClipboardRequest` to an entity.
The window module sets the clipboard each time the game writes the request.
On the web, the window module sets the clipboard in the next handler of a key or of a button.

The user pastes with Cmd+V on macOS, and with Ctrl+V on each other system.
On the web, the `paste` event of the browser is the paste.
For each paste, the window module spawns an entity with `ideatek:Paste`. An observer of `BRIDGE_ON_ENTER` hears the paste.
The window module destroys the entity in the next round. A reader that keeps the text copies it.

## The bytes of the pastes

At `init`, the window module takes `clipboard_bytes` bytes from its share of the persistent arena.
Each round writes the pastes in one half of the bytes, and the next round writes in the other half.
A paste that is too large for the free bytes of its half gets no entity. The log of the window module gets 1 line.
The window module never cuts a paste.
The manifest of the window module gives `persistent`, and `persistent` must also hold `clipboard_bytes`.

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `clipboard_bytes` | 16384 | 2 to 1073741824, an even number | The bytes for the pastes. Each round uses at most the half. |

## ideatek:ClipboardRequest

`ideatek:ClipboardRequest` asks to put a text on the clipboard.
`ideatek_clipboard_request_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The text, in UTF-8, in the share of the game. |

## ideatek:Paste

`ideatek:Paste` gives a text that the user pasted.
`ideatek_paste_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The text, in UTF-8, in `clipboard_bytes`. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ClipboardRequest` | `ideatek_clipboard_request_t` | `ideatek_clipboard_request_t_desc` | `IDEATEK_CLIPBOARD_REQUEST_FIELDS` |
| `ideatek:Paste` | `ideatek_paste_t` | `ideatek_paste_t_desc` | `IDEATEK_PASTE_FIELDS` |

```text
{ bridge_text_t text; }
{ bridge_text_t text; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ClipboardRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Paste` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/platform_abstraction/ideatek_window.h.md`, *The declarations*, tells the rules of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:ClipboardRequest` | No type. |
| The window module | `ideatek:Paste` | `ideatek:ClipboardRequest` |
| A reader | No type. | `ideatek:Paste` |
