# BC-IdeaTek-Input

The contract of input devices: the keyboard, text input, the mouse, gamepads, fingers and the focus.

## Install

The release of `BC-IdeaTek-Input` has the tag `input-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Input.zip` and `sha256.txt`. `BC-IdeaTek-Input.zip` has a folder for each target.
In `BC-IdeaTek-Input`, each folder holds `ideatek_input.h`, `ideatek_input_gamepad.h`, `ideatek_input_keyboard.h`, `ideatek_input_mouse.h`, `ideatek_input_text.h` and `ideatek_input_touch.h` in `include`.
Each folder of `BC-IdeaTek-Input.zip` also holds this readme and the licence.

`BC-IdeaTek-Input` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Input` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module includes the headers of `BC-IdeaTek-Input` that it needs:

| Header | Document |
| --- | --- |
| `ideatek_input.h` | `docs/extern/platform_abstraction/ideatek_input.h.md` |
| `ideatek_input_gamepad.h` | `docs/extern/platform_abstraction/ideatek_input_gamepad.h.md` |
| `ideatek_input_keyboard.h` | `docs/extern/platform_abstraction/ideatek_input_keyboard.h.md` |
| `ideatek_input_mouse.h` | `docs/extern/platform_abstraction/ideatek_input_mouse.h.md` |
| `ideatek_input_text.h` | `docs/extern/platform_abstraction/ideatek_input_text.h.md` |
| `ideatek_input_touch.h` | `docs/extern/platform_abstraction/ideatek_input_touch.h.md` |

The parts in the `uses` of `BC-IdeaTek-Input` are `BC-IdeaTek-World` and `BC-IdeaTek-Window`. Their readmes give their headers.
