# BC-IdeaTek-Window

The contract of the window: the window, the displays, the video modes, the drops and the clipboard.

## Install

The release of `BC-IdeaTek-Window` has the tag `window-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Window.zip` and `sha256.txt`. `BC-IdeaTek-Window.zip` has a folder for each target.
In `BC-IdeaTek-Window`, each folder holds `ideatek_window.h` and `ideatek_window_clipboard.h` in `include`.
Each folder of `BC-IdeaTek-Window.zip` also holds this readme and the licence.

`BC-IdeaTek-Window` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Window` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module includes the headers of `BC-IdeaTek-Window` that it needs:

| Header | Document |
| --- | --- |
| `ideatek_window.h` | `docs/extern/platform_abstraction/ideatek_window.h.md` |
| `ideatek_window_clipboard.h` | `docs/extern/platform_abstraction/ideatek_window_clipboard.h.md` |

The parts in the `uses` of `BC-IdeaTek-Window` are `BC-IdeaTek-World`. Their readmes give their headers.
