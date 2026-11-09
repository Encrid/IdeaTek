# BM-IdeaTek-Input-SDL3

The input module of IdeaTek on SDL3: the keyboard, the text input, the mouse, the gamepads and the fingers.

## Install

The release of `BM-IdeaTek-Input-SDL3` has the tag `input_sdl3-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Input-SDL3.zip` and `sha256.txt`. `BM-IdeaTek-Input-SDL3.zip` has a folder for each target.
In `BM-IdeaTek-Input-SDL3`, each folder holds the module `input_sdl3.dll`, `input_sdl3.so` or `input_sdl3.dylib`.
Each folder of `BM-IdeaTek-Input-SDL3.zip` also holds this readme and the licence.

The module loads the shared library of `BL-IdeaTek-SDL3` from the folder of the module.
Thus a deployment puts the module, the window module and that library in 1 folder: `docs/usage/platform_abstraction/sdl3.md`.
A project names the zip of `BM-IdeaTek-Input-SDL3` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_input_sdl3:input@0.1.0`.
Its role is the input module: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The roles*.
The component of its settings is `ideatek_input_sdl3:Settings`. It holds the settings of `docs/extern/platform_abstraction/ideatek_input.h.md`, *The settings of the input module*, and this setting of the module:

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `round_entities` | 256 | 1 to 65535 | The cap of the entities of 1 round: the events and the new gamepads. The events that get no entity stay for the next round, in their order. |

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The device events of SDL3 come on the thread of the phases. |
| `reloadable` | `false` | The devices of SDL3 are not in the storage of Bridge. |
| `spawns` | 256 | At least `round_entities`. |
| `changes` | 65536 | Less room spreads the events of a round over more rounds. |
| `persistent` | 4096 | At least the bytes of the path of `mappings`, rounded up to a multiple of 8. |
| `scratch` | 16384 | Room for the declarations and for the log lines. |
| `log_ring` | 1024 | Bytes of the log ring, 32 for each line. |
| `spans` | 65536 | The room of the lists of the 4 observers. |

- The module takes the grants that each input header gives to the input module: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The grants*.
- Its system `events` runs in its phase `first`, which the deployment maps to the phase that starts a round.
- A pair puts `ideatek_window_sdl3:window:events` before `ideatek_input_sdl3:input:events`. Thus the input module takes the key presses after the window module has seen the pastes.

### The messages

The log of the module has these lines:

| Line | Cause |
| --- | --- |
| `no input: <reason>` | SDL3 does not start its video or its gamepads. The load fails. |
| `no mappings <path>: <reason>` | SDL3 does not read the file of `mappings`. |
| `no gamepad <id>: <reason>` | SDL3 does not open a gamepad that connects. It gets no entity. |
| `more than <count> gamepads: the gamepad <id> gets no entity` | A gamepad connects while the module holds its most gamepads. |
| `the name of the gamepad <id> is longer than 378 bytes` | The name of a gamepad does not fit in `name`. |
| `no rumble on the gamepad <entity>: <reason>` | SDL3 refuses the rumble of `ideatek:GamepadRequest`. |
| `no cursor <cursor>: <reason>` | SDL3 does not make the cursor of `ideatek:MouseRequest`. The cursor of the system stays. |
| `no mouse mode <mode>: <reason>` | SDL3 refuses the mode of `ideatek:MouseRequest`. The mouse keeps its mode. |
| `no text input: <reason>` | SDL3 does not start the text input of `ideatek:TextRequest`. |

The line of a setting outside its range is in `readme.md`, *The contracts*.
