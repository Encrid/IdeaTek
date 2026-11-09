# BL-IdeaTek-SDL3-Start

The start and the stop of a subsystem of SDL3 for a module of IdeaTek, with no handler of the signals of the system.

## Install

The release of `BL-IdeaTek-SDL3-Start` has the tag `sdl3_start-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-SDL3-Start.zip` and `sha256.txt`. `BL-IdeaTek-SDL3-Start.zip` has a folder for each target.
In `BL-IdeaTek-SDL3-Start`, each folder holds the static archive `sdl3_start.a`, and `sdl3_start.h` in `include`.
Each folder of `BL-IdeaTek-SDL3-Start.zip` also holds this readme and the licence.

A module links `sdl3_start.a` and the shared library of `BL-IdeaTek-SDL3`: `docs/usage/platform_abstraction/sdl3.md`.
The archive needs the headers of SDL3 of that part to compile. It holds no code of SDL3.
A project names the zip of `BL-IdeaTek-SDL3-Start` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `sdl3_start.h`, which gives 2 functions:

| Function | Result |
| --- | --- |
| `bool sdl3_start_subsystem(SDL_InitFlags flags, char *reason, size_t size)` | Starts the subsystems of `flags`. Gives `true`, or `false` with the error of SDL3 in the `size` bytes of `reason`. |
| `void sdl3_stop_subsystem(SDL_InitFlags flags)` | Stops the subsystems of `flags`, and stops SDL3 after its last subsystem. |

- SDL3 installs no handler of the signals: the host owns them.
- At the first start of SDL3, the types of events that no part of IdeaTek takes go off.
- `sdl3_start` writes no line in a log. The module writes the line of a failure with the reason.

The rules of a caller:

1. Start and stop on the thread that starts the video of SDL3. A pinned module does this in `init` and in `shutdown`.
2. After the start, call `SDL_SetEventEnabled` with `true` for each type of event of the module.
3. After a start that gives `true`, give the same flags to the stop.
4. After a start that gives `false`, stop nothing. The start then holds no subsystem.

`BL-IdeaTek-SDL3-Start` names the part `sdl3` in `uses`.
