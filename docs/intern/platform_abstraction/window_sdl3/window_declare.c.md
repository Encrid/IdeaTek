# src/platform_abstraction/window_sdl3/window_declare.c

This file declares the 14 types of the window module, in the order of their slots: `docs/intern/platform_abstraction/window_sdl3/window_inside.h.md`, *The slots*.

## The declarations

Each type states the kind, the storage layout and `on_destroy` of its contract: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The declarations*.
`ideatek_window_sdl3:Settings` states `BRIDGE_SINGLETON`.

| Types | `on_unload` | Reason |
| --- | --- | --- |
| `ideatek:Display`, `ideatek:VideoMode`, `ideatek:Drop` and `ideatek:Paste` | `BRIDGE_SWEEP` | The module spawns these entities, and they carry nothing else. |
| Each other type | `BRIDGE_STRIP` | An entity of the game keeps the type, or the type is a singleton. |

## Functions

| Function | Result |
| --- | --- |
| `declare_types` | Declares the 14 types in the order of their slots. |
| `declare_fields` | Declares the 5 field types of the slots 0 to 4. |
| `declare_window` | Declares `ideatek:WindowRequest`, `ideatek:WindowState`, `ideatek:Display` and `ideatek:VideoMode`. |
| `declare_items` | Declares `ideatek:Drop`, `ideatek:ClipboardRequest`, `ideatek:Paste`, `ideatek:EventWait` and the settings. |
