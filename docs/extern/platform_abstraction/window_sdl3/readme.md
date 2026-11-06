# BM-IdeaTek-Window-SDL3

The window module of IdeaTek on SDL3: the window, the displays, the drops and the clipboard, as a native module of Bridge.

## Install

The release of `BM-IdeaTek-Window-SDL3` has the tag `window_sdl3-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Window-SDL3.zip` and `sha256.txt`. `BM-IdeaTek-Window-SDL3.zip` has a folder for each target.
In `BM-IdeaTek-Window-SDL3`, each folder holds the module `window_sdl3.dll`, `window_sdl3.so` or `window_sdl3.dylib`.
Each folder of `BM-IdeaTek-Window-SDL3.zip` also holds this readme and the licence.

The module links the shared library of `BL-IdeaTek-SDL3` by its name, and finds it in its own folder.
Thus a deployment puts the module and that library in 1 folder: `docs/extern/platform_abstraction/sdl3/readme.md`.
A project names the zip of `BM-IdeaTek-Window-SDL3` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_window_sdl3:window@0.1.0`.
Its role is the window module, and it also serves the clipboard: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The roles*.
Its settings are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *The settings of the window module*, and in `docs/extern/platform_abstraction/ideatek_window_clipboard.h.md`, *The bytes of the pastes*.
The component of its settings is `ideatek_window_sdl3:Settings`.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module drives the window on the thread of the phases. |
| `reloadable` | `false` | The window of SDL3 is not in the storage of Bridge. |
| `spawns` | 256 | `docs/extern/platform_abstraction/ideatek_window.h.md`, *The settings of the window module*, gives the least value. |
| `changes` | 65536 | Less room spreads the work of a round over more rounds. |
| `persistent` | 86016 | At least `drop_bytes` plus `clipboard_bytes` plus the bytes of `icon`, each rounded up to a multiple of 8. |
| `scratch` | 16384 | The bytes of the declarations and of the lines of the log. |
| `log_ring` | 1024 | The room of the lines of the log. |
| `spans` | 65536 | The room of the lists of the 2 observers. |

- The grants of the module are the grants of the window module: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The grants*.
- The module runs its system `events` in its phase `first`. The deployment maps that phase to the first phase of the round.
- A pair puts the clock module before `ideatek_window_sdl3:window:events`. A second pair puts `events` before the input module.
- The deployment loads the GPU module before the window module. Bridge stops the modules in the order of their loads, so the surface of Vulkan goes before the window.

### The messages

The module writes these lines in its log:

| Line | Cause |
| --- | --- |
| `no video: <reason>` | SDL3 does not start its video. The load fails. |
| `no window: <reason>` | SDL3 does not open the window. |
| `no icon <path>: <reason>` | SDL3 does not read the PNG file of `icon`, or the system refuses the icon. |
| `the entity <id> asks for a second window, and gets none` | A second entity gets `ideatek:WindowRequest`. |
| `the title is longer than 127 bytes, and the window shows its first <count> bytes` | The title of the request has no zero in its 128 bytes. |
| `the video mode <id> is not an exclusive mode of the display, so the window fills the display` | The exclusive mode of the request is dead, or is not a listed mode of the display. |
| `the system refuses the video mode <id>: <reason>, so the window fills the display` | SDL3 refuses the exclusive mode. |
| `the window has no handle of its surface: <reason>` | SDL3 gives no handle of the window, or no Metal view on macOS. The surface is `IDEATEK_SURFACE_NONE`. |
| `no list of the displays: <reason>` | SDL3 gives no list of the displays. |
| `more than 16 displays: the display <id> gets no entity` | The system has a 17th display. |
| `the name of the display <id> is longer than 127 bytes` | The name of a display does not fit in `name`. |
| `no video modes of the display <id>: <reason>` | SDL3 gives no list of the modes of a display. |
| `the display <id> has <count> video modes, and the module keeps the first 255` | A display has more than 255 modes. |
| `the drop <set>.<index> needs <count> bytes, and the half has <count> free bytes` | A dropped item does not fit in the half of the round. |
| `the paste needs <count> bytes, and the half has <count> free bytes` | A pasted text does not fit in the half of the round. |
| `the text at <at> with <len> bytes is not in the persistent arena` | A text of a request or of `icon` is outside the arena. |
| `no memory for a text of <count> bytes` | The module gets no memory for the copy of a text. |
| `no clipboard: <reason>` | SDL3 refuses the text of `ideatek:ClipboardRequest`. |

A setting outside its range gives the line of `readme.md`, *The contracts*.
